#include <Utils/serialize.h>
#include "physicalbody.h"

#include <iostream>

struct PhysicalBodyData {
    obj_fixed_data_len_t size;
    DrawingStyle drawing_style;
};

PhysicalBody::PhysicalBody(DataStorageReader &data)
        : drawing_style(((PhysicalBodyData*)(data.data))->drawing_style) {
    PTR_MOVE_BYTES(data.data, *(obj_fixed_data_len_t*)data.data)
    DataObjectSkipEnd(data);
    PTR_MOVE_BYTES(data.data, 1)
}

size_t PhysicalBody::GetSavedSize() const {
    return sizeof(saved_obj_id_t) + sizeof(PhysicalBodyData) + 1;
}

void PhysicalBody::SaveData(DataStorageWriter &data) const {
    ((PhysicalBodyData*)data.data)->size = sizeof(PhysicalBodyData);
    ((PhysicalBodyData*)data.data)->drawing_style = this->drawing_style;
    PUT_FIXEDONLY_NUL(data.data)
}

void PhysicalBody::SaveID(DataStorageWriter &data) const {
    PTR_APPEND(data.data, saved_obj_id_t, SAVED_OBJ_BODY)
}

