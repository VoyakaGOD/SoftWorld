#include "serialize_special.h"
#include "ser_class_enums.h"
#include <string.h>
#include <string>

size_t readShortLength(DataStorageReader &reader) {
    size_t len = 0;
    uint8_t offs = 0;
    uint8_t byte = 0;

    while ((byte & 0x80) == 0) {
        PTR_READVAL(reader.data, uint8_t, byte)
        len |= (byte & 0x7F) << offs;
        offs += 7;
    }
    return len;
}

size_t writeShortLength(DataStorageWriter &writer, size_t len) {
    while (len) {
        PTR_APPEND(writer.data, uint8_t, len & 0x7F)
        len = len >> 7;
    }
    ((uint8_t*)(writer.data))[-1] |= 0x80;
    return len;
}

size_t lenAsShortLength(size_t len) {
    size_t ret;
    while(len) {
        len = len >> 7;
        ret++;
    }
    return ret;
}

char* StringDeserialize(DataStorageReader &deser, size_t* len_ptr) {
    saved_obj_id_t obj_class;
    PTR_READVAL(deser.data, saved_obj_id_t, obj_class);
    size_t len = readShortLength(deser);

    if (obj_class != SAVED_OBJ_STR) {
        PTR_MOVE_BYTES(deser.data, -1)
        return nullptr;
    }

    if (len_ptr) {
        *len_ptr = len;
    }

    char* ret = (char*)(deser.data);
    PTR_MOVE_BYTES(deser.data, len)
    return ret;
}

size_t StringGetSavedSize(size_t len) {
    return sizeof(saved_obj_id_t) + lenAsShortLength(len) + len + 1;
}

void StringSerialize(const char* str, DataStorageWriter &writer){
    uint8_t* begin = (uint8_t*)writer.data;
    PTR_APPEND(writer.data, saved_obj_id_t, SAVED_OBJ_STR)
    size_t len = strlen(str);
    writeShortLength(writer, len);
    strcpy((char*)(writer.data), str);
    PTR_MOVE_BYTES(writer.data, len+1)
}

void StringSerialize(const void* str, DataStorageWriter &writer, size_t len){
    uint8_t* begin = (uint8_t*)writer.data;
    PTR_APPEND(writer.data, saved_obj_id_t, SAVED_OBJ_STR)
    writeShortLength(writer, len);
    memcpy((char*)(writer.data), str, len);
    PTR_MOVE_BYTES(writer.data, len+1)
}

void StringSerialize(std::string str, DataStorageWriter &writer){
    uint8_t* begin = (uint8_t*)writer.data;
    PTR_APPEND(writer.data, saved_obj_id_t, SAVED_OBJ_STR)
    writeShortLength(writer, str.length());
    char* cbuff = (char*)writer.data;
    for (int i = 0; i < str.size(); i++) {
        cbuff[i] = str[i];
    }
    cbuff[str.size()] = '\0';

    PTR_MOVE_BYTES(writer.data, str.size() + 1)
}
