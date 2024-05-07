#include <string.h>
#include <string>
#include "ser_class_enums.h"
#include "deser_helpers.h"

#include "serialize_special.h"

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

void writeShortLength(DataStorageWriter &writer, size_t len) {
    while (len) {
        WRITER_APPEND(writer, uint8_t, len & 0x7F)
        len = len >> 7;
    }
    ((uint8_t*)(writer.data))[-1] |= 0x80;
}

size_t lenAsShortLength(size_t len) {
    size_t ret = 0;
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

void StringSerialize(const char* str, DataStorageWriter &writer){
    uint8_t* begin = (uint8_t*)writer.data;
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_STR)
    size_t len = strlen(str);
    writeShortLength(writer, len+1);
    strcpy((char*)(writer.data), str);
    WRITER_MOVE_BYTES(writer, len+1)
}

void StringSerialize(const void* str, DataStorageWriter &writer, size_t len){
    uint8_t* begin = (uint8_t*)writer.data;
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_STR)
    writeShortLength(writer, len);
    memcpy((char*)(writer.data), str, len);
    WRITER_MOVE_BYTES(writer, len);
}

void StringSerialize(std::string str, DataStorageWriter &writer){
    uint8_t* begin = (uint8_t*)writer.data;
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_STR)
    writeShortLength(writer, str.length()+1);
    char* cbuff = (char*)writer.data;
    WRITER_MOVE_BYTES(writer, str.length() + 1)
    for (int i = 0; i < str.length(); i++) {
        cbuff[i] = str[i];
    }
    cbuff[str.length()] = '\0';
}

void savePkgHeader(size_t count, DataStorageWriter &writer) {
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_PKG)
    writeShortLength(writer, count);
}

int readPkgHeader(DataStorageReader &reader, size_t* size) {
    saved_obj_id_t id = *(saved_obj_id_t*)(reader.data);

    if (id != SAVED_OBJ_PKG) {
        return DESERR_INVTYPE;
    }
    WRITER_MOVE_BYTES(reader, sizeof(saved_obj_id_t));
    *size = readShortLength(reader);
    return DESERR_OK;
}
