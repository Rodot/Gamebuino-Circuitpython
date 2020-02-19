/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Scott Shawcroft
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

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/board/__init__.h"

//| :mod:`board` --- Board specific pin names
//| ========================================================
//|
//| .. module:: board
//|   :synopsis: Board specific pin names
//|
//| Common container for board base pin names. These will vary from board to
//| board so don't expect portability when using this module.
//|
//| .. warning:: The board module varies by board. The APIs documented here may or may not be
//|              available on a specific board.

//| .. function:: I2C()
//|
//|   Returns the `busio.I2C` object for the board designated SDA and SCL pins. It is a singleton.
//|

#if BOARD_I2C
mp_obj_t board_i2c(void) {
    mp_obj_t singleton = common_hal_board_get_i2c();
    if (singleton != NULL) {
        return singleton;
    }
    assert_pin_free(DEFAULT_I2C_BUS_SDA);
    assert_pin_free(DEFAULT_I2C_BUS_SCL);
    return common_hal_board_create_i2c();
}
#else
mp_obj_t board_i2c(void) {
    mp_raise_NotImplementedError_varg(translate("No default %q bus"), MP_QSTR_I2C);
    return NULL;
}
#endif
MP_DEFINE_CONST_FUN_OBJ_0(board_i2c_obj, board_i2c);


//| .. function:: SPI()
//|
//|   Returns the `busio.SPI` object for the board designated SCK, MOSI and MISO pins. It is a
//|   singleton.
//|
#if BOARD_SPI
mp_obj_t board_spi(void) {
    mp_obj_t singleton = common_hal_board_get_spi();
    if (singleton != NULL) {
        return singleton;
    }
    assert_pin_free(DEFAULT_SPI_BUS_SCK);
    assert_pin_free(DEFAULT_SPI_BUS_MOSI);
    assert_pin_free(DEFAULT_SPI_BUS_MISO);
    return common_hal_board_create_spi();
}
#else
mp_obj_t board_spi(void) {
    mp_raise_NotImplementedError_varg(translate("No default %q bus"), MP_QSTR_SPI);
    return NULL;
}
#endif
MP_DEFINE_CONST_FUN_OBJ_0(board_spi_obj, board_spi);

//| .. function:: UART()
//|
//|   Returns the `busio.UART` object for the board designated TX and RX pins. It is a singleton.
//|
#if BOARD_UART
mp_obj_t board_uart(void) {
    mp_obj_t singleton = common_hal_board_get_uart();
    if (singleton != NULL) {
        return singleton;
    }

    assert_pin_free(DEFAULT_UART_BUS_RX);
    assert_pin_free(DEFAULT_UART_BUS_TX);

    return common_hal_board_create_uart();
}
#else
mp_obj_t board_uart(void) {
    mp_raise_NotImplementedError_varg(translate("No default %q bus"), MP_QSTR_SPI);
    return NULL;
}
#endif
MP_DEFINE_CONST_FUN_OBJ_0(board_uart_obj, board_uart);

const mp_obj_module_t board_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&board_module_globals,
};
