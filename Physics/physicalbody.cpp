#include <Serialize/serialize.h>
#include <Serialize/deserialize.h>
#include "physicalbody.h"

#include <iostream>

struct PhysicalBodyData {
    obj_fixed_data_len_t size;
};

PhysicalBody::PhysicalBody(DataStorageReader &data) {
    PTR_MOVE_BYTES(data.data, *(obj_fixed_data_len_t*)data.data)
    DataObjectSkipEnd(data);
    PTR_MOVE_BYTES(data.data, 1)
}

size_t PhysicalBody::GetSavedSize() const {
    return sizeof(saved_obj_id_t) + sizeof(PhysicalBodyData) + 1;
}

void PhysicalBody::SaveData(DataStorageWriter &data) const {
    ((PhysicalBodyData*)data.data)->size = sizeof(PhysicalBodyData);
    PUT_FIXEDONLY_NUL(data.data)
}

