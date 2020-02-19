/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Scott Shawcroft for Adafruit Industries
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

#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/displayio/__init__.h"
#include "shared-bindings/displayio/Bitmap.h"
#include "shared-bindings/displayio/ColorConverter.h"
#include "shared-bindings/displayio/Display.h"
#include "shared-bindings/displayio/FourWire.h"
#include "shared-bindings/displayio/Group.h"
#include "shared-bindings/displayio/OnDiskBitmap.h"
#include "shared-bindings/displayio/Palette.h"
#include "shared-bindings/displayio/ParallelBus.h"
#include "shared-bindings/displayio/Shape.h"
#include "shared-bindings/displayio/TileGrid.h"

//| :mod:`displayio` --- Native display driving
//| =========================================================================
//|
//| .. module:: displayio
//|   :synopsis: Native helpers for driving displays
//|   :platform: SAMD21, SAMD51, nRF52
//|
//| The `displayio` module contains classes to manage display output
//| including synchronizing with refresh rates and partial updating.
//|
//| Libraries
//|
//| .. toctree::
//|     :maxdepth: 3
//|
//|     Bitmap
//|     ColorConverter
//|     Display
//|     FourWire
//|     Group
//|     OnDiskBitmap
//|     Palette
//|     ParallelBus
//|     Shape
//|     TileGrid
//|


//| .. function:: release_displays()
//|
//|   Releases any actively used displays so their busses and pins can be used again. This will also
//|   release the builtin display on boards that have one. You will need to reinitialize it yourself
//|   afterwards.
//|
//|   Use this once in your code.py if you initialize a display. Place it right before the
//|   initialization so the display is active as long as possible.
//|
STATIC mp_obj_t displayio_release_displays(void) {
    common_hal_displayio_release_displays();
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_0(displayio_release_displays_obj, displayio_release_displays);

STATIC const mp_rom_map_elem_t displayio_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_displayio) },
    { MP_ROM_QSTR(MP_QSTR_Bitmap), MP_ROM_PTR(&displayio_bitmap_type) },
    { MP_ROM_QSTR(MP_QSTR_ColorConverter), MP_ROM_PTR(&displayio_colorconverter_type) },
    { MP_ROM_QSTR(MP_QSTR_Display), MP_ROM_PTR(&displayio_display_type) },
    { MP_ROM_QSTR(MP_QSTR_Group), MP_ROM_PTR(&displayio_group_type) },
    { MP_ROM_QSTR(MP_QSTR_OnDiskBitmap), MP_ROM_PTR(&displayio_ondiskbitmap_type) },
    { MP_ROM_QSTR(MP_QSTR_Palette), MP_ROM_PTR(&displayio_palette_type) },
    { MP_ROM_QSTR(MP_QSTR_Shape), MP_ROM_PTR(&displayio_shape_type) },
    { MP_ROM_QSTR(MP_QSTR_TileGrid), MP_ROM_PTR(&displayio_tilegrid_type) },

    { MP_ROM_QSTR(MP_QSTR_FourWire), MP_ROM_PTR(&displayio_fourwire_type) },
    { MP_ROM_QSTR(MP_QSTR_ParallelBus), MP_ROM_PTR(&displayio_parallelbus_type) },

    { MP_ROM_QSTR(MP_QSTR_release_displays), MP_ROM_PTR(&displayio_release_displays_obj) },
};

STATIC MP_DEFINE_CONST_DICT(displayio_module_globals, displayio_module_globals_table);

const mp_obj_module_t displayio_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&displayio_module_globals,
};
