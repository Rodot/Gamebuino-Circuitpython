#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

extern int somevar;
void gambeuino_meta_test(void);
void gamebuino_meta_begin(void);

STATIC mp_obj_t gbm_life(void) { return mp_obj_new_int(somevar); }
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_life_obj, gbm_life);

STATIC mp_obj_t gbm_loader(void) {
    ((void(*)(void))(*((uint32_t*)0x3FF4)))();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_loader_obj, gbm_loader);

STATIC mp_obj_t gbm_test(void) { gambeuino_meta_test(); return mp_const_none; }
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_test_obj, gbm_test);

STATIC mp_obj_t gbm_begin(void) { gamebuino_meta_begin(); return mp_const_none; }
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gbm_begin_obj, gbm_begin);

STATIC const mp_map_elem_t mymodule_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_gamebuino_meta) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_life), (mp_obj_t)&gbm_life_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_loader), (mp_obj_t)&gbm_loader_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_test), (mp_obj_t)&gbm_test_obj },
    { MP_OBJ_NEW_QSTR(MP_QSTR_begin), (mp_obj_t)&gbm_begin_obj },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_mymodule_globals,
    mymodule_globals_table
);

const mp_obj_module_t mp_module_gamebuino_meta = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_mymodule_globals,
};
