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

#include "shared-bindings/displayio/Group.h"

#include <stdint.h>

#include "lib/utils/context_manager_helpers.h"
#include "py/binary.h"
#include "py/objproperty.h"
#include "py/objtype.h"
#include "py/runtime.h"
#include "supervisor/shared/translate.h"

//| .. currentmodule:: displayio
//|
//| :class:`Group` -- Group together sprites and subgroups
//| ==========================================================================
//|
//| Manage a group of sprites and groups and how they are inter-related.
//|
//| .. class:: Group(*, max_size=4, scale=1, x=0, y=0)
//|
//|   Create a Group of a given size and scale. Scale is in one dimension. For example, scale=2
//|   leads to a layer's pixel being 2x2 pixels when in the group.
//|
//|   :param int max_size: The maximum group size.
//|   :param int scale: Scale of layer pixels in one dimension.
//|   :param int x: Initial x position within the parent.
//|   :param int y: Initial y position within the parent.
//|
STATIC mp_obj_t displayio_group_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_max_size, ARG_scale, ARG_x, ARG_y };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_max_size, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 4} },
        { MP_QSTR_scale, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 1} },
        { MP_QSTR_x, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0} },
        { MP_QSTR_y, MP_ARG_INT | MP_ARG_KW_ONLY, {.u_int = 0} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    mp_int_t max_size = args[ARG_max_size].u_int;
    if (max_size < 1) {
        mp_raise_ValueError_varg(translate("%q must be >= 1"), MP_QSTR_max_size);
    }

    mp_int_t scale = args[ARG_scale].u_int;
    if (scale < 1) {
        mp_raise_ValueError_varg(translate("%q must be >= 1"), MP_QSTR_scale);
    }

    displayio_group_t *self = m_new_obj(displayio_group_t);
    self->base.type = &displayio_group_type;
    common_hal_displayio_group_construct(self, max_size, scale, args[ARG_x].u_int, args[ARG_y].u_int);

    return MP_OBJ_FROM_PTR(self);
}

// Helper to ensure we have the native super class instead of a subclass.
displayio_group_t* native_group(mp_obj_t group_obj) {
    mp_obj_t native_group = mp_instance_cast_to_native_base(group_obj, &displayio_group_type);
    if (native_group == MP_OBJ_NULL) {
        mp_raise_ValueError_varg(translate("Must be a %q subclass."), MP_QSTR_Group);
    }
    mp_obj_assert_native_inited(native_group);
    return MP_OBJ_TO_PTR(native_group);
}

//|   .. attribute:: scale
//|
//|     Scales each pixel within the Group in both directions. For example, when scale=2 each pixel
//|     will be represented by 2x2 pixels.
//|
STATIC mp_obj_t displayio_group_obj_get_scale(mp_obj_t self_in) {
    displayio_group_t *self = native_group(self_in);
    return MP_OBJ_NEW_SMALL_INT(common_hal_displayio_group_get_scale(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(displayio_group_get_scale_obj, displayio_group_obj_get_scale);

STATIC mp_obj_t displayio_group_obj_set_scale(mp_obj_t self_in, mp_obj_t scale_obj) {
    displayio_group_t *self = native_group(self_in);

    mp_int_t scale = mp_obj_get_int(scale_obj);
    if (scale < 1) {
        mp_raise_ValueError_varg(translate("%q must be >= 1"), MP_QSTR_scale);
    }
    common_hal_displayio_group_set_scale(self, scale);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_set_scale_obj, displayio_group_obj_set_scale);

const mp_obj_property_t displayio_group_scale_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&displayio_group_get_scale_obj,
              (mp_obj_t)&displayio_group_set_scale_obj,
              (mp_obj_t)&mp_const_none_obj},
};

//|   .. attribute:: x
//|
//|     X position of the Group in the parent.
//|
STATIC mp_obj_t displayio_group_obj_get_x(mp_obj_t self_in) {
    displayio_group_t *self = native_group(self_in);
    return MP_OBJ_NEW_SMALL_INT(common_hal_displayio_group_get_x(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(displayio_group_get_x_obj, displayio_group_obj_get_x);

STATIC mp_obj_t displayio_group_obj_set_x(mp_obj_t self_in, mp_obj_t x_obj) {
    displayio_group_t *self = native_group(self_in);

    mp_int_t x = mp_obj_get_int(x_obj);
    common_hal_displayio_group_set_x(self, x);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_set_x_obj, displayio_group_obj_set_x);

const mp_obj_property_t displayio_group_x_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&displayio_group_get_x_obj,
              (mp_obj_t)&displayio_group_set_x_obj,
              (mp_obj_t)&mp_const_none_obj},
};

//|   .. attribute:: y
//|
//|     Y position of the Group in the parent.
//|
STATIC mp_obj_t displayio_group_obj_get_y(mp_obj_t self_in) {
    displayio_group_t *self = native_group(self_in);
    return MP_OBJ_NEW_SMALL_INT(common_hal_displayio_group_get_y(self));
}
MP_DEFINE_CONST_FUN_OBJ_1(displayio_group_get_y_obj, displayio_group_obj_get_y);

STATIC mp_obj_t displayio_group_obj_set_y(mp_obj_t self_in, mp_obj_t y_obj) {
    displayio_group_t *self = native_group(self_in);

    mp_int_t y = mp_obj_get_int(y_obj);
    common_hal_displayio_group_set_y(self, y);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_set_y_obj, displayio_group_obj_set_y);

const mp_obj_property_t displayio_group_y_obj = {
    .base.type = &mp_type_property,
    .proxy = {(mp_obj_t)&displayio_group_get_y_obj,
              (mp_obj_t)&displayio_group_set_y_obj,
              (mp_obj_t)&mp_const_none_obj},
};

//|   .. method:: append(layer)
//|
//|     Append a layer to the group. It will be drawn above other layers.
//|
STATIC mp_obj_t displayio_group_obj_append(mp_obj_t self_in, mp_obj_t layer) {
    displayio_group_t *self = native_group(self_in);
    common_hal_displayio_group_insert(self, common_hal_displayio_group_get_len(self), layer);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_append_obj, displayio_group_obj_append);

//|   .. method:: insert(index, layer)
//|
//|     Insert a layer into the group.
//|
STATIC mp_obj_t displayio_group_obj_insert(mp_obj_t self_in, mp_obj_t index_obj, mp_obj_t layer) {
    displayio_group_t *self = native_group(self_in);
    size_t index = mp_get_index(&displayio_group_type, common_hal_displayio_group_get_len(self), index_obj, false);
    common_hal_displayio_group_insert(self, index, layer);
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_3(displayio_group_insert_obj, displayio_group_obj_insert);


//|   .. method:: index(layer)
//|
//|     Returns the index of the first copy of layer. Raises ValueError if not found.
//|
STATIC mp_obj_t displayio_group_obj_index(mp_obj_t self_in, mp_obj_t layer) {
    displayio_group_t *self = native_group(self_in);
    mp_int_t index = common_hal_displayio_group_index(self, layer);
    if (index < 0) {
        mp_raise_ValueError(translate("object not in sequence"));
    }
    return MP_OBJ_NEW_SMALL_INT(index);
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_index_obj, displayio_group_obj_index);

//|   .. method:: pop(i=-1)
//|
//|     Remove the ith item and return it.
//|
STATIC mp_obj_t displayio_group_obj_pop(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_i };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_i, MP_ARG_INT, {.u_int = -1} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args - 1, pos_args + 1, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    displayio_group_t *self = native_group(pos_args[0]);

    size_t index = mp_get_index(&displayio_group_type,
                                common_hal_displayio_group_get_len(self),
                                MP_OBJ_NEW_SMALL_INT(args[ARG_i].u_int),
                                false);
    return common_hal_displayio_group_pop(self, index);
}
MP_DEFINE_CONST_FUN_OBJ_KW(displayio_group_pop_obj, 1, displayio_group_obj_pop);


//|   .. method:: remove(layer)
//|
//|     Remove the first copy of layer. Raises ValueError if it is not present.
//|
STATIC mp_obj_t displayio_group_obj_remove(mp_obj_t self_in, mp_obj_t layer) {
    mp_obj_t index = displayio_group_obj_index(self_in, layer);
    displayio_group_t *self = native_group(self_in);

    common_hal_displayio_group_pop(self, MP_OBJ_SMALL_INT_VALUE(index));
    return mp_const_none;
}
MP_DEFINE_CONST_FUN_OBJ_2(displayio_group_remove_obj, displayio_group_obj_remove);

//|   .. method:: __len__()
//|
//|     Returns the number of layers in a Group
//|
STATIC mp_obj_t group_unary_op(mp_unary_op_t op, mp_obj_t self_in) {
    displayio_group_t *self = native_group(self_in);
    uint16_t len = common_hal_displayio_group_get_len(self);
    switch (op) {
        case MP_UNARY_OP_BOOL: return mp_obj_new_bool(len != 0);
        case MP_UNARY_OP_LEN: return MP_OBJ_NEW_SMALL_INT(len);
        default: return MP_OBJ_NULL; // op not supported
    }
}

//|   .. method:: __getitem__(index)
//|
//|     Returns the value at the given index.
//|
//|     This allows you to::
//|
//|       print(group[0])
//|
//|   .. method:: __setitem__(index, value)
//|
//|     Sets the value at the given index.
//|
//|     This allows you to::
//|
//|       group[0] = sprite
//|
//|   .. method:: __delitem__(index)
//|
//|     Deletes the value at the given index.
//|
//|     This allows you to::
//|
//|       del group[0]
//|
STATIC mp_obj_t group_subscr(mp_obj_t self_in, mp_obj_t index_obj, mp_obj_t value) {
    displayio_group_t *self = native_group(self_in);

    if (MP_OBJ_IS_TYPE(index_obj, &mp_type_slice)) {
        mp_raise_NotImplementedError(translate("Slices not supported"));
    } else {
        size_t index = mp_get_index(&displayio_group_type, common_hal_displayio_group_get_len(self), index_obj, false);

        if (value == MP_OBJ_SENTINEL) {
            // load
            return common_hal_displayio_group_get(self, index);
        } else if (value == MP_OBJ_NULL) {
            common_hal_displayio_group_pop(self, index);
        } else {
            common_hal_displayio_group_set(self, index, value);
        }
    }
    return mp_const_none;
}

STATIC const mp_rom_map_elem_t displayio_group_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_scale), MP_ROM_PTR(&displayio_group_scale_obj) },
    { MP_ROM_QSTR(MP_QSTR_x), MP_ROM_PTR(&displayio_group_x_obj) },
    { MP_ROM_QSTR(MP_QSTR_y), MP_ROM_PTR(&displayio_group_y_obj) },
    { MP_ROM_QSTR(MP_QSTR_append), MP_ROM_PTR(&displayio_group_append_obj) },
    { MP_ROM_QSTR(MP_QSTR_insert), MP_ROM_PTR(&displayio_group_insert_obj) },
    { MP_ROM_QSTR(MP_QSTR_index), MP_ROM_PTR(&displayio_group_index_obj) },
    { MP_ROM_QSTR(MP_QSTR_pop), MP_ROM_PTR(&displayio_group_pop_obj) },
    { MP_ROM_QSTR(MP_QSTR_remove), MP_ROM_PTR(&displayio_group_remove_obj) },
};
STATIC MP_DEFINE_CONST_DICT(displayio_group_locals_dict, displayio_group_locals_dict_table);

const mp_obj_type_t displayio_group_type = {
    { &mp_type_type },
    .name = MP_QSTR_Group,
    .make_new = displayio_group_make_new,
    .subscr = group_subscr,
    .unary_op = group_unary_op,
    .getiter = mp_obj_new_generic_iterator,
    .locals_dict = (mp_obj_dict_t*)&displayio_group_locals_dict,
};
