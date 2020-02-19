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
#include "shared-bindings/busio/SPI.h"
#include "shared-bindings/displayio/FourWire.h"
#include "shared-module/displayio/__init__.h"
#include "shared-module/displayio/mipi_constants.h"
#include "tick.h"

displayio_fourwire_obj_t board_display_obj;

#define DELAY 0x80

uint8_t display_init_sequence[] = {
    0x01, 0 | DELAY, 150, // SWRESET
    0x11, 0 | DELAY, 255, // SLPOUT
    0x36, 1, 0x08,        // _MADCTL bottom to top refresh in vsync aligned order.
    0x3a, 1, 0x55,        // COLMOD - 16bit color
    0x21, 0 | DELAY, 10,  // _INVON
    0x13, 0 | DELAY, 10,  // _NORON
    0x29, 0 | DELAY, 255, // _DISPON
};

void board_init(void) {
    busio_spi_obj_t* spi = &displays[0].fourwire_bus.inline_bus;
    common_hal_busio_spi_construct(spi, &pin_PB13, &pin_PB15, NULL);
    common_hal_busio_spi_never_reset(spi);

    displayio_fourwire_obj_t* bus = &displays[0].fourwire_bus;
    bus->base.type = &displayio_fourwire_type;
    common_hal_displayio_fourwire_construct(bus,
        spi,
        &pin_PB05, // TFT_DC Command or data
        &pin_PB06, // TFT_CS Chip select
        &pin_PB07, // TFT_RST Reset
        60000000);

    displayio_display_obj_t* display = &displays[0].display;
    display->base.type = &displayio_display_type;
    common_hal_displayio_display_construct(display,
        bus,
        320, // Width (after rotation)
        240, // Height (after rotation)
        0, // column start
        0, // row start
        90, // rotation
        16, // Color depth
        MIPI_COMMAND_SET_COLUMN_ADDRESS, // Set column command
        MIPI_COMMAND_SET_PAGE_ADDRESS, // Set row command
        MIPI_COMMAND_WRITE_MEMORY_START, // Write memory command
        0x37, // set vertical scroll command
        display_init_sequence,
        sizeof(display_init_sequence),
        &pin_PA01,  // backlight pin
        1.0f, // brightness (ignored)
        true, // auto_brightness
        false, // single_byte_bounds
        false); // data_as_commands
}

bool board_requests_safe_mode(void) {
    return false;
}

void reset_board(void) {
}
