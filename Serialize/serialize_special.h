#pragma once

#include <Utils/fileworks.h>
#include <string>

///SERIALIZABLE String SAVED_OBJ_STR special nosaveid=1
///SERIALIZABLE invalid SAVED_OBJ_INVALID special nosaveid=1

size_t readShortLength(DataStorageReader &reader);
size_t writeShortLength(DataStorageWriter &writer, size_t len);
size_t lenAsShortLength(size_t len);


size_t StringGetSavedSize(size_t len);
char* StringDeserialize(DataStorageReader &deser, size_t* len_ptr = nullptr);

void StringSerialize(const char* str, DataStorageWriter &writer);
void StringSerialize(const void* str, DataStorageWriter &writer, size_t len);
void StringSerialize(std::string str, DataStorageWriter &writer);
