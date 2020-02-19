/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Scott Shawcroft for Adafruit Industries
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

#include "shared-bindings/fontio/Glyph.h"

#include <stdint.h>

//| .. currentmodule:: fontio
//|
//| :class:`Glyph` -- Storage of glyph info
//| ==========================================================================
//|
//| .. class:: Glyph(bitmap, tile_index, width, height, dx, dy, shift_x, shift_y)
//|
//|   Named tuple used to capture a single glyph and its attributes.
//|
//|   :param displayio.Bitmap bitmap: the bitmap including the glyph
//|   :param int tile_index: the tile index within the bitmap
//|   :param int width: the width of the glyph's bitmap
//|   :param int height: the height of the glyph's bitmap
//|   :param int dx: x adjustment to the bitmap's position
//|   :param int dy: y adjustment to the bitmap's position
//|   :param int shift_x: the x difference to the next glyph
//|   :param int shift_y: the y difference to the next glyph
//|
const mp_obj_namedtuple_type_t fontio_glyph_type = {
    .base = {
        .base = {
            .type = &mp_type_type
        },
        .name = MP_QSTR_Glyph,
        .print = namedtuple_print,
        .make_new = namedtuple_make_new,
        .unary_op = mp_obj_tuple_unary_op,
        .binary_op = mp_obj_tuple_binary_op,
        .attr = namedtuple_attr,
        .subscr = mp_obj_tuple_subscr,
        .getiter = mp_obj_tuple_getiter,
        .parent = &mp_type_tuple,
    },
    .n_fields = 8,
    .fields = {
        MP_QSTR_bitmap,
        MP_QSTR_tile_index,
        MP_QSTR_width,
        MP_QSTR_height,
        MP_QSTR_dx,
        MP_QSTR_dy,
        MP_QSTR_shift_x,
        MP_QSTR_shift_y
    },
};
