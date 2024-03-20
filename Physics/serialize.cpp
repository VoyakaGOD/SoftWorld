#include "physicalbody.h"
#include "ghostbody.h"

#include "serialize.h"

enum SavedObjId {
    SAVED_OBJ_NONE = 0,
    SAVED_OBJ_BODY,
    SAVED_OBJ_PALLETE_ITEM,
    SAVED_OBJ_SCENE
};

typedef uint8_t obj_class_type;
typedef uint16_t body_class_type;

#define PTR_MOVE_BYTES(ptr, offs) (ptr) = ((uint8_t*)(ptr)) + offs;
#define PTR_APPEND(ptr, type, val) {(*(type*)(ptr)) = val; (ptr) = ((type*)ptr) + 1;}
#define PTR_READVAL(ptr, type, val) {val = (*(type*)(ptr)); (ptr) = ((type*)ptr) + 1;}

void bodySerialize(PhysicalBody *body, DataStorageWriter &ser) {
    BodyClass bclass = body->GetClass();
    size_t size = body->GetDataSize();

    void* target = ser.PrepareDataBuf(sizeof(obj_class_type) + sizeof(body_class_type) + size);
    PTR_APPEND(target, obj_class_type, SAVED_OBJ_BODY);

    PTR_APPEND(target, body_class_type, bclass);

    body->GetData(target);
    PTR_MOVE_BYTES(target, size)
}

PhysicalBody* bodyDeserialize(DataStorageReader &deser) {
    PTR_MOVE_BYTES(deser.data, sizeof(obj_class_type));
    body_class_type bclass;
    PTR_READVAL(deser.data, body_class_type, bclass);
    PhysicalBody* ret = nullptr;
    switch (bclass) {
        case BODY_CLASS_UNDEF:
            printf("undefined body class\n");
            break;
        case BODY_CLASS_GHOST:
            printf("load ghost body\n");
            ret = new GhostBody((GhostBodyData*)(deser.data));
            break;
        default:
            printf("unknown body class\n");
            break;
    }
    if (ret) {
        PTR_MOVE_BYTES(deser.data, ret->GetDataSize())
    }
    return ret;
}

