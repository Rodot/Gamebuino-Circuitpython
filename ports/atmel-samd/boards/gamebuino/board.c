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
#include "py/mpstate.h"
#include "shared-bindings/microcontroller/Processor.h"
#include "hal/include/hal_spi_m_sync.h"
#include "hal/include/hpl_spi_m_sync.h"
#include "shared-bindings/busio/SPI.h"
#include "shared-bindings/microcontroller/Pin.h"
#include "samd/dma.h"
#include "samd/sercom.h"
#include "supervisor/shared/stack.h"
#include "supervisor/filesystem.h"
#include "supervisor/shared/autoreload.h"
#include "supervisor/port.h"
#include "shared-module/board/__init__.h"
#include <string.h>

#define BOARD_CS_BTN_PORT                 (1)
#define BOARD_CS_BTN_PIN                  (3)

#define BOARD_CS_SD_PORT                 (0)
#define BOARD_CS_SD_PIN                  (27)

#define CS_BTN_INIT()   PORT->Group[BOARD_CS_BTN_PORT].DIRSET.reg = (1UL<<BOARD_CS_BTN_PIN)
#define CS_BTN_H()   PORT->Group[BOARD_CS_BTN_PORT].OUT.reg |= (1UL<<BOARD_CS_BTN_PIN)
#define CS_BTN_L()   PORT->Group[BOARD_CS_BTN_PORT].OUT.reg &= ~(1UL<<BOARD_CS_BTN_PIN)

#define CS_SD_INIT()   PORT->Group[BOARD_CS_SD_PORT].DIRSET.reg = (1UL<<BOARD_CS_SD_PIN)
#define CS_SD_H()   PORT->Group[BOARD_CS_SD_PORT].OUT.reg |= (1UL<<BOARD_CS_SD_PIN)
#define CS_SD_L()   PORT->Group[BOARD_CS_SD_PORT].OUT.reg &= ~(1UL<<BOARD_CS_SD_PIN)

void board_init(void) {
    WDT->CTRL.bit.ENABLE = 0;
    CS_SD_INIT();
    CS_SD_H();
}

bool board_requests_safe_mode(void) {
    return false;
}

void gamebuino_meta_reset(void);
void gamebuino_meta_begin(void);
void gamebuino_meta_titlescreen(void);
void start_mp(supervisor_allocation* heap);
void stop_mp(void);
bool firstReset = true;
void reset_board(void) {
    if (firstReset) {
        stack_resize();
        filesystem_flush();
        supervisor_allocation* heap = allocate_remaining_memory();
        start_mp(heap);
        
        // code here
        gamebuino_meta_titlescreen();
        
        stop_mp();
        free_memory(heap);
        reset_port();
        reset_board_busses();
//        reset_board();
        gamebuino_meta_reset();
    }
    firstReset = false;
    gamebuino_meta_reset();
    for (uint8_t i = 0; i < DYNAMIC_MEM_ROOTPOINTERS_SIZE; i++) {
        MP_STATE_PORT(dynamic_mem_rootpointers[i]) = 0;
    }
}

//void shared_modules_random_seed(mp_uint_t);

void gamebuino_meta_pick_random_seed(void) {
//    unsigned int seed = 42;
//    shared_modules_random_seed(seed);
}

void* gb_malloc(size_t size) {
    void* ptr = m_malloc(size, true);
    for (uint8_t i = 0; i < DYNAMIC_MEM_ROOTPOINTERS_SIZE; i++) {
        if (MP_STATE_PORT(dynamic_mem_rootpointers[i]) == 0) {
            MP_STATE_PORT(dynamic_mem_rootpointers[i]) = ptr;
            break;
        }
    }
    return ptr;
}

void gb_free(void* ptr) {
    m_free(ptr);
    for (uint8_t i = 0; i < DYNAMIC_MEM_ROOTPOINTERS_SIZE; i++) {
        if (MP_STATE_PORT(dynamic_mem_rootpointers[i]) == ptr) {
            MP_STATE_PORT(dynamic_mem_rootpointers[i]) = 0;
        }
    }
}

static busio_spi_obj_t spi_obj;
bool inited_spi = false;
void spi_init(void) {
    if (inited_spi) {
        return;
    }
    common_hal_busio_spi_construct(&spi_obj, &pin_PB11, &pin_PB10, &pin_PA12);
    common_hal_busio_spi_never_reset(&spi_obj);
    inited_spi = true;
}

// button functions
void gamebuino_meta_buttons_init(void) {
    spi_init();
    CS_BTN_INIT();
    CS_BTN_H();
}
uint8_t gamebuino_meta_buttons_update(void) {
    CS_BTN_L();
    common_hal_busio_spi_configure(&spi_obj, 12000000, 0, 0, 8);
    uint8_t r;
    common_hal_busio_spi_read(&spi_obj, &r, 1, 0xFF);
    CS_BTN_H();
    return r;
}

/*
#define DMA_DESC_COUNT (3)
uint8_t next_desc_channel = 0;
volatile uint32_t dma_desc_free_count = DMA_DESC_COUNT;
void DMAC_Handler(void) {
    __disable_irq();
    uint8_t channel = DMAC->INTPEND.bit.ID;
    if (channel >= DMA_DESC_COUNT) {
        __enable_irq();
        return;
    }
    DMAC->CHID.bit.ID = channel;
    uint8_t flags = DMAC->CHINTFLAG.reg;
    if (flags & DMAC_CHINTENCLR_TERR) {
        // error
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TERR;
    } else if(flags & DMAC_CHINTENCLR_TCMPL) {
        // complete
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_TCMPL;
        dma_desc_free_count++;
        if (dma_desc_free_count < DMA_DESC_COUNT-1) {
            DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_CMD_RESUME;
        }
    } else if (flags & DMAC_CHINTENCLR_SUSP) {
        // suspend
        DMAC->CHINTFLAG.reg = DMAC_CHINTENCLR_SUSP;
        if (dma_desc_free_count < DMA_DESC_COUNT) {
            DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_CMD_RESUME;
        }
    }
    __enable_irq();
}
*/
// tft functions
void gamebuino_meta_tft_spi_begin(void) {
    spi_init();
    /*
    next_desc_channel = 0;
    for (uint8_t i = 0; i < DMA_DESC_COUNT; i++) {
        DmacDescriptor* desc = dma_descriptor(i);
        bool stepSel = false;
        uint32_t stepSize = 0;
        desc->BTCTRL.bit.VALID    = true;
        desc->BTCTRL.bit.EVOSEL   = 0;
        desc->BTCTRL.bit.BLOCKACT = 0;
        desc->BTCTRL.bit.BEATSIZE = 0;
        desc->BTCTRL.bit.SRCINC   = true;
        desc->BTCTRL.bit.DSTINC   = false;
        desc->BTCTRL.bit.STEPSEL  = stepSel;
        desc->BTCTRL.bit.STEPSIZE = stepSize;
        desc->BTCNT.reg           = 0;
        desc->SRCADDR.reg         = 0;
        
        desc->DSTADDR.reg         = (uint32_t)(void *)(SERCOM4->SPI.DATA.reg);
        desc->DESCADDR.reg        = (uint32_t)dma_descriptor((i+1)%DMA_DESC_COUNT);
        
        DMAC->CHID.bit.ID         = i;
        DMAC->CHCTRLA.bit.ENABLE  = 0;
        DMAC->CHCTRLA.bit.SWRST   = 1;
        DMAC->SWTRIGCTRL.reg     &= (uint32_t)(~(1ul << i));
        DMAC->CHCTRLB.bit.LVL     = 0;
        DMAC->CHCTRLB.bit.TRIGSRC = 0x0A;
        DMAC->CHCTRLB.bit.TRIGACT = DMAC_CHCTRLB_TRIGACT_BEAT_Val;
    }
    NVIC_EnableIRQ(DMAC_IRQn);
    NVIC_SetPriority(DMAC_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
    */
}
void gamebuino_meta_tft_spi_begin_transaction(void) {
    common_hal_busio_spi_configure(&spi_obj, 24000000, 0, 0, 8);
}
void gamebuino_meta_tft_spi_end_transaction(void) {
    // do nothing for now
}
void gamebuino_meta_tft_spi_transfer(uint8_t d) {
    common_hal_busio_spi_write(&spi_obj, &d, 1);
}
void gamebuino_meta_tft_wait_for_transfers_done(void) {
    //while (dma_desc_free_count < DMA_DESC_COUNT);
    sercom_dma_transfer_wait(spi_obj.spi_desc.dev.prvt);
}
void gamebuino_meat_tft_wait_for_desc_available(const uint32_t num) {
    //while (dma_desc_free_count < num);
    gamebuino_meta_tft_wait_for_transfers_done();
}
void gamebuino_meta_tft_send_buffer(uint16_t* buf, uint16_t size) {
//    common_hal_busio_spi_write(&spi_obj, (uint8_t*)buf, size*2);
    sercom_dma_write_nowait(spi_obj.spi_desc.dev.prvt, (uint8_t*)buf, size*2);
    /*
    bool start = false;
    uint32_t count = size*2;
    void* src = buf;
    
    gamebuino_meat_tft_wait_for_desc_available(1);
    // change descriptor
    DmacDescriptor* desc = dma_descriptor(next_desc_channel);
    // from adafruit zerodma
    uint8_t bytesPerBeat = 1; // Beat transfer size IN BYTES
    if(count) desc->BTCNT.reg = count;
    desc->SRCADDR.reg = (uint32_t)src;
    if(desc->BTCTRL.bit.SRCINC){
        if(desc->BTCTRL.bit.STEPSEL) desc->SRCADDR.reg += desc->BTCNT.reg * bytesPerBeat * (1<<desc->BTCTRL.bit.STEPSIZE);
        else desc->SRCADDR.reg += desc->BTCNT.reg * bytesPerBeat;
    }
    DMAC->CHID.bit.ID  = next_desc_channel;
	DMAC->CHCTRLA.reg |= DMAC_CHCTRLA_ENABLE;
    __disable_irq();
    dma_desc_free_count--;
    DMAC->CHID.bit.ID    = next_desc_channel;
    DMAC->CHCTRLB.reg |= DMAC_CHCTRLB_CMD_RESUME;
    start = !(DMAC->CHCTRLA.bit.ENABLE);
    __enable_irq();
    uint8_t this_channel = next_desc_channel;
    next_desc_channel = (next_desc_channel + 1) % DMA_DESC_COUNT;
    if (start) {
        __disable_irq();
        uint8_t interruptMask = 0b00000111;
        DMAC->CHID.bit.ID = this_channel;
        DMAC->CHINTENSET.reg = DMAC_CHINTENSET_MASK &  interruptMask;
        DMAC->CHINTENCLR.reg = DMAC_CHINTENCLR_MASK & ~interruptMask;
        DMAC->CHCTRLA.bit.ENABLE = 1; // Enable the transfer channel
        __enable_irq();
    }
    */
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

void yield(void);
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
