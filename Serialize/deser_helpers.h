#pragma once
#include <Utils/fileworks.h>

class SerializableObject {
    public:
    virtual void SaveID(DataStorageWriter &data) const = 0;
    virtual size_t GetSavedSize() const = 0;
    virtual void SaveData(DataStorageWriter &data) const = 0;
};

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

