#include <Physics/physicalbody.h>
#include <Physics/ghostbody.h>
#include <UI/palleteitem.h>
#include <UI/pallete.h>

#include <Utils/serialize.h>

PhysicalBody* physicalBodyDeserialize(DataStorageReader &deser) {
    saved_obj_id_t obj_class;
    PTR_READVAL(deser.data, saved_obj_id_t, obj_class);
    if (obj_class != SAVED_OBJ_BODY){
        if (obj_class == SAVED_OBJ_NONE) {
            PTR_MOVE_BYTES(deser.data, -1)
        }
        return nullptr;
    }

    saved_obj_id_t bclass;
    PTR_READVAL(deser.data, saved_obj_id_t, bclass);

    switch (bclass) {
        case BODY_CLASS_UNDEF:
            return nullptr;
        case BODY_CLASS_GHOST:
            return new GhostBody(deser);
        default:
            return nullptr;
    }
    return nullptr;
}

PalleteItem* palleteItemDeserialize(QWidget* parent, Qt::WindowFlags wflags, SceneView* scene, DataStorageReader &deser) {
    saved_obj_id_t obj_class;
    PTR_READVAL(deser.data, saved_obj_id_t, obj_class);
    if (obj_class != SAVED_OBJ_PALLETE_ITEM) {
        PhysicalBody* body = physicalBodyDeserialize(deser);
        return new PalleteItem(parent, wflags, scene, body, "*Unnamed*");
    }

    return new PalleteItem(parent, wflags, scene, deser);
}

char* stringDeserialize(DataStorageReader &deser) {
    saved_obj_id_t obj_class;
    PTR_READVAL(deser.data, saved_obj_id_t, obj_class);

    if (obj_class != SAVED_OBJ_STR) {
        if (obj_class == SAVED_OBJ_NONE) {
            PTR_MOVE_BYTES(deser.data, -1)
        }
        return nullptr;
    }
    char* ret = (char*)(deser.data);
    while (*(char*)(deser.data)) {PTR_MOVE_BYTES(deser.data, 1)}
    return ret;
}

void DataObjectSkipEnd(DataStorageReader &deser) {
    while (*(saved_obj_id_t*)deser.data != SAVED_OBJ_NONE) {
        DataObjectSkip(deser);
    }
}

void DataObjectSkip(DataStorageReader &deser) {
    int depth = 0;
    saved_obj_id_t obj_class;
    PTR_READVAL(deser.data, saved_obj_id_t, obj_class);

    while (obj_class & 0x80) {
        PTR_READVAL(deser.data, saved_obj_id_t, obj_class);
        if (obj_class == SAVED_OBJ_NONE) {
            if (depth > 0)
                PTR_MOVE_BYTES(deser.data, 1)
            break;
        }
        PTR_MOVE_BYTES(deser.data, 1)
        depth++;
    }

    for (int i = 0; i < depth; i++) {
        obj_fixed_data_len_t len = *(obj_fixed_data_len_t*)(deser.data);
        PTR_MOVE_BYTES(deser.data, len);
        DataObjectSkipEnd(deser);
    }

}

size_t StringSerialize(const char* str, void* buff){
    PTR_APPEND(buff, saved_obj_id_t, SAVED_OBJ_STR)
    strcpy((char*)buff, str);
    return sizeof(saved_obj_id_t) + strlen(str) + 1;
}

size_t StringSerialize(std::string str, void* buff){
    PTR_APPEND(buff, saved_obj_id_t, SAVED_OBJ_STR)
    char* cbuff = (char*)buff;
    for (int i = 0; i < str.size(); i++) {
        cbuff[i] = str[i];
    }
    cbuff[str.size()] = '\0';
    return sizeof(saved_obj_id_t) + str.size() + 1;
}
