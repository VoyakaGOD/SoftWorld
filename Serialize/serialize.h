#pragma once
#include <Utils/fileworks.h>
#include "deser_helpers.h"
#include "ser_class_enums.h"

inline static void saveObj(DataStorageWriter &ser, SerializableObject &obj) {
    obj.SaveID(ser);
    obj.SaveData(ser);
}

inline static void serializeObj(DataStorageWriter &ser, SerializableObject &obj) {
    ser.PrepareDataBuf(obj.GetSavedSize());
    saveObj(ser, obj);
}

#define PUT_FIXEDONLY_NUL(ptr) {\
    obj_fixed_data_len_t fixed_size = *(obj_fixed_data_len_t*)ptr; \
    PTR_MOVE_BYTES(ptr, fixed_size)        \
    *(saved_obj_id_t*)ptr = SAVED_OBJ_NONE;\
    PTR_MOVE_BYTES(ptr, 1) \
}
