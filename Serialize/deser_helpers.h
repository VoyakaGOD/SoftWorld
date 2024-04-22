#pragma once
#include <Utils/fileworks.h>

class SerializableObject {
    // if deserialization type is "simple", then static SerializableObject* Deserialize(FileReadInterface &) is needed
    // if deserialization type is "inplace", then static int Deserialize(SerializableObject*, FileReadInterface &) is needed
    // if this has its own class, then ///SERIALIZABLE <class> <enum> <deser type> [nosaveid=1, deser=<deser method>] comment is needed as a hint for codegen
    // you may add a usable deserialization method in deserialize.cpp
    public:
    virtual void SaveID(DataStorageWriter &data) const = 0;
    virtual size_t GetSavedSize() const = 0;
    virtual void SaveData(DataStorageWriter &data) const = 0;
};

#define GETDATA(reader, type, field) \
    ((offsetof(type, field) + sizeof(type::field) <= (((type*)(reader.data))->size)) ? \
    (((type*)(reader.data))->field) : default_data.field)

enum DeserTblEntryType {
    DESER_TBL_NONE,
    DESER_TBL_SIMPLE,
    DESER_TBL_SPECIAL,
    DESER_TBL_INPLACE
};

enum DeserializeError {
    DESERR_OK = 0,
    DESERR_NOCLASS,
    DESERR_BOUNDS,
    DESERR_INVTYPE,
    DESERR_BADDATA,
    DESERR_ERRNO
};
extern DeserializeError last_deserialize_error;

const char* deserrErrorMsg(DeserializeError err);

struct DeserTblEntry {
    DeserTblEntry* parent;
    const char* const name;
    uint8_t type;
    union {
        DeserTblEntry* e_base;
        SerializableObject* (*e_simple)(DataStorageReader &);
        int (*e_inplace)(SerializableObject*, DataStorageReader &);
    };
};

struct DeserTblBaseEntry {
    DeserTblEntry* parent;
    const char* const name;
    uint8_t size;
    uint8_t bsize;
    DeserTblEntry* e_norm;
    DeserTblBaseEntry* e_base;
};

