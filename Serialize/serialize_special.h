#pragma once

#include <Utils/fileworks.h>
#include <string>

///SERIALIZABLE String SAVED_OBJ_STR special nosaveid=1
///SERIALIZABLE Package SAVED_OBJ_PKG special nosaveid=1
///SERIALIZABLE invalid SAVED_OBJ_INVALID special nosaveid=1

size_t readShortLength(DataStorageReader &reader);
void writeShortLength(DataStorageWriter &writer, size_t len);
size_t lenAsShortLength(size_t len);


inline static size_t StringGetSavedSize(size_t len) {
    return sizeof(saved_obj_id_t) + lenAsShortLength(len) + len;
}

char* StringDeserialize(DataStorageReader &deser, size_t* len_ptr = nullptr);

void StringSerialize(const char* str, DataStorageWriter &writer);
void StringSerialize(const void* str, DataStorageWriter &writer, size_t len);
void StringSerialize(std::string str, DataStorageWriter &writer);

inline static size_t getPkgHeaderSize(size_t count) {
    return sizeof(saved_obj_id_t) + lenAsShortLength(count);
}

void savePkgHeader(size_t count, DataStorageWriter &writer);
int readPkgHeader(DataStorageReader &reader, size_t* size);
