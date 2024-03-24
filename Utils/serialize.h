#pragma once

#include <string>
#include "fileworks.h"
#include <Physics/physicalbody.h>
#include <UI/palleteitem.h>

enum SavedObjId {
    SAVED_OBJ_NONE = 0,
    SAVED_OBJ_INVALID,
    SAVED_OBJ_PALLETE_ITEM = 0x1F,
    SAVED_OBJ_SCENE,
    SAVED_OBJ_STR,

    SAVED_OBJ_BODY = 0x81, // base class
};

#define SAVE_OBJ(ser, obj) {(obj).SaveID(ser); (obj).SaveData(ser);}

#define SERIALIZE_OBJ(ser, obj) {\
    ser.PrepareDataBuf((obj).GetSavedSize()); \
    SAVE_OBJ(ser, obj) \
}

#define PUT_FIXEDONLY_NUL(ptr) {\
    obj_fixed_data_len_t fixed_size = *(obj_fixed_data_len_t*)ptr; \
    PTR_MOVE_BYTES(ptr, fixed_size)        \
    *(saved_obj_id_t*)ptr = SAVED_OBJ_NONE;\
    PTR_MOVE_BYTES(ptr, 1) \
}

PhysicalBody* physicalBodyDeserialize(DataStorageReader &deser);
PalleteItem* palleteItemDeserialize(QWidget* parent, Qt::WindowFlags wflags, SceneView* scene, DataStorageReader &deser);

char* stringDeserialize(DataStorageReader &deser);

void DataObjectSkipEnd(DataStorageReader &deser);
void DataObjectSkip(DataStorageReader &deser);

size_t StringSerialize(const char* str, void* buff);
size_t StringSerialize(std::string str, void* buff);
