#include "py/nlr.h"
#include "py/obj.h"
#include "py/runtime.h"
#include "py/binary.h"

extern int somevar;

STATIC mp_obj_t gamebuino_meta_life(void) {
    return mp_obj_new_int(somevar);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(gamebuino_meta_life_obj, gamebuino_meta_life);

STATIC const mp_map_elem_t mymodule_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_gamebuino_meta) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_life), (mp_obj_t)&gamebuino_meta_life_obj },
};

STATIC MP_DEFINE_CONST_DICT (
    mp_module_mymodule_globals,
    mymodule_globals_table
);

const mp_obj_module_t mp_module_gamebuino_meta = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&mp_module_mymodule_globals,
};
