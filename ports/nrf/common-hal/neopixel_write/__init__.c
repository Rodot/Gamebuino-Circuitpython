/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 hathach for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/mphal.h"
#include "shared-bindings/neopixel_write/__init__.h"
#include "nrf_pwm.h"

#include "tick.h"

// https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp
// [[[Begin of the Neopixel NRF52 EasyDMA implementation
//                                    by the Hackerspace San Salvador]]]
// This technique uses the PWM peripheral on the NRF52. The PWM uses the
// EasyDMA feature included on the chip. This technique loads the duty
// cycle configuration for each cycle when the PWM is enabled. For this
// to work we need to store a 16 bit configuration for each bit of the
// RGB(W) values in the pixel buffer.
// Comparator values for the PWM were hand picked and are guaranteed to
// be 100% organic to preserve freshness and high accuracy. Current
// parameters are:
//   * PWM Clock: 16Mhz
//   * Minimum step time: 62.5ns
//   * Time for zero in high (T0H): 0.31ms
//   * Time for one in high (T1H): 0.75ms
//   * Cycle time:  1.25us
//   * Frequency: 800Khz
// For 400Khz we just double the calculated times.
// ---------- BEGIN Constants for the EasyDMA implementation -----------
// The PWM starts the duty cycle in LOW. To start with HIGH we
// need to set the 15th bit on each register.

// WS2812 (rev A) timing is 0.35 and 0.7us
//#define MAGIC_T0H               5UL | (0x8000) // 0.3125us
//#define MAGIC_T1H              12UL | (0x8000) // 0.75us

// WS2812B (rev B) timing is 0.4 and 0.8 us
#define MAGIC_T0H               6UL | (0x8000) // 0.375us
#define MAGIC_T1H              13UL | (0x8000) // 0.8125us
#define CTOPVAL                20UL            // 1.25us

// ---------- END Constants for the EasyDMA implementation -------------
//
// If there is no device available an alternative cycle-counter
// implementation is tried.
// The nRF52840 runs with a fixed clock of 64Mhz. The alternative
// implementation is the same as the one used for the Teensy 3.0/1/2 but
// with the Nordic SDK HAL & registers syntax.
// The number of cycles was hand picked and is guaranteed to be 100%
// organic to preserve freshness and high accuracy.
// ---------- BEGIN Constants for cycle counter implementation ---------
#define CYCLES_800_T0H  18  // ~0.36 uS
#define CYCLES_800_T1H  41  // ~0.76 uS
#define CYCLES_800      71  // ~1.25 uS

// ---------- END of Constants for cycle counter implementation --------

// find a free PWM device, which is not enabled and has no connected pins
static NRF_PWM_Type* find_free_pwm (void) {
    NRF_PWM_Type* PWM[] = {
            NRF_PWM0, NRF_PWM1, NRF_PWM2
#ifdef NRF_PWM3
            , NRF_PWM3
#endif
    };

    for ( size_t device = 0; device < ARRAY_SIZE(PWM); device++ ) {
        if ( (PWM[device]->ENABLE == 0) &&
             (PWM[device]->PSEL.OUT[0] & PWM_PSEL_OUT_CONNECT_Msk) && (PWM[device]->PSEL.OUT[1] & PWM_PSEL_OUT_CONNECT_Msk) &&
             (PWM[device]->PSEL.OUT[2] & PWM_PSEL_OUT_CONNECT_Msk) && (PWM[device]->PSEL.OUT[3] & PWM_PSEL_OUT_CONNECT_Msk) ) {
            return PWM[device];
        }
    }

    return NULL;
}

uint64_t next_start_tick_ms = 0;
uint32_t next_start_tick_us = 1000;

void common_hal_neopixel_write (const digitalio_digitalinout_obj_t* digitalinout, uint8_t *pixels, uint32_t numBytes) {
    // To support both the SoftDevice + Neopixels we use the EasyDMA
    // feature from the NRF25. However this technique implies to
    // generate a pattern and store it on the memory. The actual
    // memory used in bytes corresponds to the following formula:
    //              totalMem = numBytes*8*2+(2*2)
    // The two additional bytes at the end are needed to reset the
    // sequence.
    //
    // If there is not enough memory, we will fall back to cycle counter
    // using DWT

#define PATTERN_SIZE(numBytes) (numBytes * 8 * sizeof(uint16_t) + 2 * sizeof(uint16_t))

    uint32_t pattern_size = PATTERN_SIZE(numBytes);
    uint16_t* pixels_pattern = NULL;
    bool pattern_on_heap = false;

    // Use the stack to store 1 pixels worth of PWM data for the status led. uint32_t to ensure alignment.
    // Make it at least as big as PATTERN_SIZE(3), for one pixel of RGB data.
    // PATTERN_SIZE is a multiple of 4, so we don't need round up to make sure one_pixel is large enough.
    uint32_t one_pixel[PATTERN_SIZE(3)/sizeof(uint32_t)];

    NRF_PWM_Type* pwm = find_free_pwm();

    // only malloc if there is PWM device available
    if ( pwm != NULL ) {
        if (pattern_size <= sizeof(one_pixel)) {
            pixels_pattern = (uint16_t *) one_pixel;
        } else {
            pixels_pattern = (uint16_t *) m_malloc_maybe(pattern_size, false);
            pattern_on_heap = true;
        }
    }

    // Wait to make sure we don't append onto the last transmission.
    wait_until(next_start_tick_ms, next_start_tick_us);

    // Use the identified device to choose the implementation
    // If a PWM device is available use DMA
    if ( (pixels_pattern != NULL) && (pwm != NULL) ) {
        uint16_t pos = 0;  // bit position

        for ( uint16_t n = 0; n < numBytes; n++ ) {
            uint8_t pix = pixels[n];

            for ( uint8_t mask = 0x80; mask > 0; mask >>= 1 ) {
                pixels_pattern[pos] = (pix & mask) ? MAGIC_T1H : MAGIC_T0H;
                pos++;
            }
        }

        // Zero padding to indicate the end of sequence
        pixels_pattern[pos++] = 0 | (0x8000);  // Seq end
        pixels_pattern[pos++] = 0 | (0x8000);  // Seq end

        // Set the wave mode to count UP
        // Set the PWM to use the 16MHz clock
        // Setting of the maximum count
        // but keeping it on 16Mhz allows for more granularity just
        // in case someone wants to do more fine-tuning of the timing.
        nrf_pwm_configure(pwm, NRF_PWM_CLK_16MHz, NRF_PWM_MODE_UP, CTOPVAL);

        // Disable loops, we want the sequence to repeat only once
        nrf_pwm_loop_set(pwm, 0);

        // On the "Common" setting the PWM uses the same pattern for the
        // for supported sequences. The pattern is stored on half-word of 16bits
        nrf_pwm_decoder_set(pwm, PWM_DECODER_LOAD_Common, PWM_DECODER_MODE_RefreshCount);

        // Pointer to the memory storing the pattern
        nrf_pwm_seq_ptr_set(pwm, 0, pixels_pattern);

        // Calculation of the number of steps loaded from memory.
        nrf_pwm_seq_cnt_set(pwm, 0, pattern_size / sizeof(uint16_t));

        // The following settings are ignored with the current config.
        nrf_pwm_seq_refresh_set(pwm, 0, 0);
        nrf_pwm_seq_end_delay_set(pwm, 0, 0);

        // The Neopixel implementation is a blocking algorithm. DMA
        // allows for non-blocking operation. To "simulate" a blocking
        // operation we enable the interruption for the end of sequence
        // and block the execution thread until the event flag is set by
        // the peripheral.
        //    pwm->INTEN |= (PWM_INTEN_SEQEND0_Enabled<<PWM_INTEN_SEQEND0_Pos);

        // PSEL must be configured before enabling PWM
        nrf_pwm_pins_set(pwm, (uint32_t[]) {digitalinout->pin->number, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL} );

        // Enable the PWM
        nrf_pwm_enable(pwm);

        // After all of this and many hours of reading the documentation
        // we are ready to start the sequence...
        nrf_pwm_event_clear(pwm, NRF_PWM_EVENT_SEQEND0);
        nrf_pwm_task_trigger(pwm, NRF_PWM_TASK_SEQSTART0);

        // But we have to wait for the flag to be set.
        while ( !nrf_pwm_event_check(pwm, NRF_PWM_EVENT_SEQEND0) ) {
#ifdef MICROPY_VM_HOOK_LOOP
            MICROPY_VM_HOOK_LOOP
#endif
        }

        // Before leave we clear the flag for the event.
        nrf_pwm_event_clear(pwm, NRF_PWM_EVENT_SEQEND0);

        // We need to disable the device and disconnect
        // all the outputs before leave or the device will not
        // be selected on the next call.
        // TODO: Check if disabling the device causes performance issues.
        nrf_pwm_disable(pwm);
        nrf_pwm_pins_set(pwm, (uint32_t[]) {0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL, 0xFFFFFFFFUL} );

        if (pattern_on_heap) {
            m_free(pixels_pattern);
        }

    } // End of DMA implementation
    // ---------------------------------------------------------------------
    else {
        // Fall back to DWT
        // If you are using the Bluetooth SoftDevice we advise you to not disable
        // the interrupts. Disabling the interrupts even for short periods of time
        // causes the SoftDevice to stop working.
        // Disable the interrupts only in cases where you need high performance for
        // the LEDs and if you are not using the EasyDMA feature.
        __disable_irq();

        uint32_t decoded_pin = digitalinout->pin->number;
        NRF_GPIO_Type* port = nrf_gpio_pin_port_decode(&decoded_pin);

        uint32_t pinMask = ( 1UL << decoded_pin );

        uint32_t CYCLES_X00 = CYCLES_800;
        uint32_t CYCLES_X00_T1H = CYCLES_800_T1H;
        uint32_t CYCLES_X00_T0H = CYCLES_800_T0H;

        // Enable DWT in debug core
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

        // Tries to re-send the frame if is interrupted by the SoftDevice.
        while ( 1 ) {
            uint8_t *p = pixels;

            uint32_t cycStart = DWT->CYCCNT;
            uint32_t cyc = 0;

            for ( uint16_t n = 0; n < numBytes; n++ ) {
                uint8_t pix = *p++;

                for ( uint8_t mask = 0x80; mask; mask >>= 1 ) {
                    while ( DWT->CYCCNT - cyc < CYCLES_X00 )
                        ;
                    cyc = DWT->CYCCNT;

                    port->OUTSET |= pinMask;

                    if ( pix & mask ) {
                        while ( DWT->CYCCNT - cyc < CYCLES_X00_T1H )
                            ;
                    } else {
                        while ( DWT->CYCCNT - cyc < CYCLES_X00_T0H )
                            ;
                    }

                    port->OUTCLR |= pinMask;
                }
            }
            while ( DWT->CYCCNT - cyc < CYCLES_X00 )
                ;

            // If total time longer than 25%, resend the whole data.
            // Since we are likely to be interrupted by SoftDevice
            if ( (DWT->CYCCNT - cycStart) < (8 * numBytes * ((CYCLES_X00 * 5) / 4)) ) {
                break;
            }

            // re-send need 300us delay
            mp_hal_delay_us(300);
        }

        // Enable interrupts again
        __enable_irq();
    }

    // Update the next start.
    current_tick(&next_start_tick_ms, &next_start_tick_us);
    if (next_start_tick_us < 100) {
        next_start_tick_ms += 1;
        next_start_tick_us = 100 - next_start_tick_us;
    } else {
        next_start_tick_us -= 100;
    }
}
