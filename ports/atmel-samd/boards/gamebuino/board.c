/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Scott Shawcroft for Adafruit Industries
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

#include "boards/board.h"
#include "mpconfigboard.h"
#include "hal/include/hal_gpio.h"
#include "py/obj.h"
#include "shared-bindings/microcontroller/Processor.h"
#include "hal/include/hal_spi_m_sync.h"
#include "hal/include/hpl_spi_m_sync.h"
#include <string.h>


#define BOARD_MOSI_PORT                   (1)
#define BOARD_MOSI_PIN                    (10)

#define BOARD_MISO_PORT                   (0)
#define BOARD_MISO_PIN                    (12)

#define BOARD_SCK_PORT                    (1)
#define BOARD_SCK_PIN                     (11)

#define BOARD_CS_BTN_PORT                 (1)
#define BOARD_CS_BTN_PIN                  (3)

#define DO_INIT()  PORT->Group[BOARD_MISO_PORT].DIRCLR.reg = (1UL<<BOARD_MISO_PIN); PORT->Group[BOARD_MISO_PORT].PINCFG[BOARD_MISO_PIN].reg = PORT_PINCFG_INEN|PORT_PINCFG_PULLEN; PORT->Group[BOARD_MISO_PORT].OUTSET.reg = (1UL << BOARD_MISO_PIN)
#define DO         (PORT->Group[BOARD_MISO_PORT].IN.reg & (1UL<<BOARD_MISO_PIN))

#define DI_INIT()       PORT->Group[BOARD_MOSI_PORT].DIRSET.reg = (1UL<<BOARD_MOSI_PIN)
#define DI_H()          PORT->Group[BOARD_MOSI_PORT].OUTSET.reg = (1UL<<BOARD_MOSI_PIN)
#define DI_L()          PORT->Group[BOARD_MOSI_PORT].OUTCLR.reg = (1UL<<BOARD_MOSI_PIN)

#define CK_INIT()       PORT->Group[BOARD_SCK_PORT].DIRSET.reg = (1UL<<BOARD_SCK_PIN)
#define CK_H()          PORT->Group[BOARD_SCK_PORT].OUTSET.reg = (1UL<<BOARD_SCK_PIN)
#define CK_L() PORT->Group[BOARD_SCK_PORT].OUTCLR.reg = (1UL<<BOARD_SCK_PIN)

#define CS_BTN_INIT()   PORT->Group[BOARD_CS_BTN_PORT].DIRSET.reg = (1UL<<BOARD_CS_BTN_PIN)
#define CS_BTN_H()   PORT->Group[BOARD_CS_BTN_PORT].OUTSET.reg = (1UL<<BOARD_CS_BTN_PIN)
#define CS_BTN_L()   PORT->Group[BOARD_CS_BTN_PORT].OUTCLR.reg = (1UL<<BOARD_CS_BTN_PIN)

void board_init(void)
{
    WDT->CTRL.bit.ENABLE = 0;
    DO_INIT();
    DI_INIT();
    CK_INIT();
}

bool board_requests_safe_mode(void) {
    return false;
}

void reset_board(void) {
}

void shared_modules_random_seed(mp_uint_t);

void gamebuino_meta_pick_random_seed(void) {
    unsigned int seed = 42;
    shared_modules_random_seed(seed);
}

uint32_t membuff[(64*80*2 + 1024) / 4];
uint32_t ramSize = 0;

bool gamebuino_meta_is_inited(void);

void* gb_malloc(size_t size) {
    if (gamebuino_meta_is_inited()) {
        return m_malloc(size, true);
    }
    void* ptr = (void*)(&membuff) + ramSize;
    ramSize += (size + 3) / 4;
    return ptr;
}

void gb_free(void* ptr) {
    return m_free(ptr);
}

// button functions
void gamebuino_meta_buttons_init(void) {
    DO_INIT();
    DI_INIT();
    CK_INIT();
    CS_BTN_INIT();
    CS_BTN_H();
}
uint8_t gamebuino_meta_buttons_update(void) {
    CS_BTN_L();
    uint8_t r;
    r = 0;   if (DO) r++;   /* bit7 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit6 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit5 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit4 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit3 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit2 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit1 */
    CK_H(); CK_L();
    r <<= 1; if (DO) r++;   /* bit0 */
    CK_H(); CK_L();
    CS_BTN_H();
    return r;
}

// tft functions
void gamebuino_meta_tft_spi_begin(void) {
    /*
    Sercom* sercom = SERCOM4;
    uint8_t sercom_index = 4;
    uint8_t clock_pad = 3;
    uint8_t mosi_pad = 2;
    uint8_t miso_pad = 0;
    uint32_t mosi_pinmux = MUX_F;
    uint32_t miso_pinmux = MUX_F;
    uint32_t clock_pinmux = MUX_F;
    uint8_t dopo = samd_peripherals_get_spi_dopo(clock_pad, mosi_pad);
    samd_peripherals_sercom_clock_init(sercom, sercom_index);
    spi_m_sync_descriptor spi_desc;
    spi_m_sync_init(&spi_desc, sercom);
    hri_sercomspi_write_CTRLA_DOPO_bf(sercom, dopo);
    hri_sercomspi_write_CTRLA_DIPO_bf(sercom, miso_pad);
    uint8_t baud_value = samd_peripherals_spi_baudrate_to_baud_reg_value(12000000);
    spi_m_sync_set_baudrate(&spi_desc, baud_value)
    */
    DO_INIT();
    DI_INIT();
    CK_INIT();
}
void gamebuino_meta_tft_spi_begin_transaction(void) {
    // do nothing for now
}
void gamebuino_meta_tft_spi_end_transaction(void) {
    // do nothing for now
}
void gamebuino_meta_tft_spi_transfer(uint8_t d) {
    if (d & 0x80) DI_H(); else DI_L();      /* bit7 */
    CK_H(); CK_L();
    if (d & 0x40) DI_H(); else DI_L();      /* bit6 */
    CK_H(); CK_L();
    if (d & 0x20) DI_H(); else DI_L();      /* bit5 */
    CK_H(); CK_L();
    if (d & 0x10) DI_H(); else DI_L();      /* bit4 */
    CK_H(); CK_L();
    if (d & 0x08) DI_H(); else DI_L();      /* bit3 */
    CK_H(); CK_L();
    if (d & 0x04) DI_H(); else DI_L();      /* bit2 */
    CK_H(); CK_L();
    if (d & 0x02) DI_H(); else DI_L();      /* bit1 */
    CK_H(); CK_L();
    if (d & 0x01) DI_H(); else DI_L();      /* bit0 */
    CK_H(); CK_L();
}
void gamebuino_meta_tft_send_buffer(uint16_t* buf, uint16_t size) {
    uint8_t* buff = (uint8_t*)buf;
    for (uint32_t i = 0; i < size*2; i++) {
        gamebuino_meta_tft_spi_transfer(buff[i]);
    }
}
void gamebuino_meta_tft_wait_for_transfers_done(void) {
    // do nothing for now
}
void gamebuino_meat_tft_wait_for_desc_available(const uint32_t num) {
    gamebuino_meta_tft_wait_for_transfers_done();
}


// delay functions
extern volatile uint64_t ticks_ms;
uint32_t millis(void) {
    return ticks_ms;
}

uint32_t micros(void) {
    uint32_t ticks, ticks2;
    uint32_t pend, pend2;
    uint32_t count, count2;

    ticks2  = SysTick->VAL;
    pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
    count2  = ticks_ms ;

    do
    {
        ticks=ticks2;
        pend=pend2;
        count=count2;
        ticks2  = SysTick->VAL;
        pend2   = !!(SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)  ;
        count2  = ticks_ms ;
    } while ((pend != pend2) || (count != count2) || (ticks < ticks2));

    return ((count+pend) * 1000) + (((SysTick->LOAD  - ticks)*(1048576/(common_hal_mcu_processor_get_frequency()/1000000)))>>20) ;
}

static void __empty(void) {}
void yield(void) __attribute__ ((weak, alias("__empty")));

void delay(uint32_t ms) {
    if (ms == 0) {
        return;
    }
    uint32_t start = ticks_ms;
    do {
        yield();
    } while (ticks_ms - start < ms);
}

void __cxa_pure_virtual(void) { while(1); }
void __cxa_deleted_virtual(void) { while(1); }
