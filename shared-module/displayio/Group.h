/*
 * This file is part of the Micro Python project, http://micropython.org/
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

#ifndef MICROPY_INCLUDED_SHARED_MODULE_DISPLAYIO_GROUP_H
#define MICROPY_INCLUDED_SHARED_MODULE_DISPLAYIO_GROUP_H

#include <stdbool.h>
#include <stdint.h>

#include "py/obj.h"
#include "shared-module/displayio/area.h"

typedef struct {
    mp_obj_t native;
    mp_obj_t original;
} displayio_group_child_t;

typedef struct {
    mp_obj_base_t base;
    displayio_group_child_t* children;
    int16_t x;
    int16_t y;
    uint16_t scale;
    uint16_t size;
    uint16_t max_size;
    bool item_removed;
    bool in_group;
    displayio_buffer_transform_t absolute_transform;
    displayio_area_t dirty_area; // Catch all for changed area
} displayio_group_t;

void displayio_group_construct(displayio_group_t* self, displayio_group_child_t* child_array, uint32_t max_size, uint32_t scale, mp_int_t x, mp_int_t y);
bool displayio_group_get_previous_area(displayio_group_t *group, displayio_area_t* area);
bool displayio_group_fill_area(displayio_group_t *group, const displayio_area_t* area, uint32_t* mask, uint32_t *buffer);
void displayio_group_update_transform(displayio_group_t *group, const displayio_buffer_transform_t* parent_transform);
void displayio_group_finish_refresh(displayio_group_t *self);
displayio_area_t* displayio_group_get_refresh_areas(displayio_group_t *self, displayio_area_t* tail);

#endif // MICROPY_INCLUDED_SHARED_MODULE_DISPLAYIO_GROUP_H
