#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

void gamebuino_meta_begin(void);
bool gamebuino_meta_update(void);
void gamebuino_meta_display_clear(void);
void gamebuino_meta_display_println(const char* str);

STATIC mp_obj_t gbm_loader(void) {
    ((void(*)(void))(*((uint32_t*)0x3FF4)))();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_loader_obj, gbm_loader);

STATIC mp_obj_t gbm_begin(void) { gamebuino_meta_begin(); return mp_const_none; }
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_begin_obj, gbm_begin);

STATIC mp_obj_t gbm_update(void) { return mp_obj_new_bool(gamebuino_meta_update()); }
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_update_obj, gbm_update);

STATIC mp_obj_t gbm_display_clear(void) { gamebuino_meta_display_clear(); return mp_const_none; } 
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_display_clear_obj, gbm_display_clear);

STATIC mp_obj_t gbm_display_println(mp_obj_t str) { gamebuino_meta_display_println(mp_obj_str_get_str(str)); return mp_const_none; }
STATIC MP_DEFINE_CONST_FUN_OBJ_1(gbm_display_println_obj, gbm_display_println);


STATIC const mp_map_elem_t gbm_display_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_clear), (mp_obj_t)&gbm_display_clear_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_println), (mp_obj_t)&gbm_display_println_obj },
};
STATIC MP_DEFINE_CONST_DICT (
    mp_dict_gamebuino_meta_display,
    gbm_display_table
);
const mp_obj_module_t mp_module_gamebuino_meta_display = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_dict_gamebuino_meta_display,
};

STATIC const mp_map_elem_t gbm_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_gamebuino_meta) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_loader), (mp_obj_t)&gbm_loader_obj },
    
    { MP_OBJ_NEW_QSTR(MP_QSTR_begin), (mp_obj_t)&gbm_begin_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_update), (mp_obj_t)&gbm_update_obj },
    
    { MP_OBJ_NEW_QSTR(MP_QSTR_display), (mp_obj_t)(&mp_module_gamebuino_meta_display) },
};
STATIC MP_DEFINE_CONST_DICT (
    mp_module_gamebuino_meta_globals,
    gbm_globals_table
);
const mp_obj_module_t mp_module_gamebuino_meta = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_gamebuino_meta_globals,
};
