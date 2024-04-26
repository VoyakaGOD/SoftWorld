#include "desertab.h"
#include <Physics/physicalbody.h>
#include <UI/palleteitem.h>
#include <UI/pallete.h>
#include "deserialize.h"

DeserializeError last_deserialize_error;

const char* deserrErrorMsg(DeserializeError err) {
    switch(err) {
        case DESERR_OK:
            return "No error";
        case DESERR_NOCLASS:
            return "Unknown object type";
        case DESERR_BOUNDS:
            return "Unexpected end of file";
        case DESERR_INVTYPE:
            return "Incompatible object file";
        case DESERR_BADDATA:
            return "File corrupt or incompatible format";
        case DESERR_ERRNO:
            return strerror(errno);
        default:
            return "Shit happened";
    }
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

    if (obj_class == SAVED_OBJ_STR) {
        size_t len = readShortLength(deser);
        READER_MOVE_BYTES(deser, len, return;)
        return;
    }
    if (obj_class == SAVED_OBJ_PKG) {
        size_t len = readShortLength(deser);
        for(int i = 0; i < len; i++) {
            DataObjectSkip(deser);
        }
        return;
    }

    while (obj_class & 0x80) {
        PTR_READVAL(deser.data, saved_obj_id_t, obj_class);
        if (obj_class == SAVED_OBJ_NONE) {
            if (depth > 0)
                READER_MOVE_BYTES(deser, sizeof(saved_obj_id_t), return;)
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

DeserTblEntry* getDeserEntry(DataStorageReader &reader, void* reader_restore) {
    DeserTblEntry* table = obj_deser_tbl;
    DeserTblBaseEntry* base = obj_deser_tbl_b;
    uint8_t tabl_size = obj_deser_tbl_size;
    uint8_t tabl_bsize = obj_deser_tbl_size_b;
    saved_obj_id_t obj_class;

    while (true) {
        READER_READVAL(reader, saved_obj_id_t, obj_class,
             {last_deserialize_error = DESERR_BOUNDS; return nullptr;})

        if (obj_class < 128) {
            break;
        }
        obj_class -= 128;

        if (obj_class >= tabl_bsize) {
            reader.data = reader_restore;
            return nullptr;
        }

        tabl_size = base[obj_class].size;
        tabl_bsize = base[obj_class].bsize;
        table = base[obj_class].e_norm;
        base  = base[obj_class].e_base;
    }
    if (obj_class >= tabl_size) {
        last_deserialize_error = DESERR_NOCLASS;
        return nullptr;
    }

    return table + obj_class;
}

SerializableObject* deserializeSimple(DataStorageReader &reader, DeserTblEntry* entry, void* reader_restore) {
    if (!entry) {
        reader.data = reader_restore;
        last_deserialize_error = DESERR_NOCLASS;
        return nullptr;
    }

    switch (entry->type){
        case DESER_TBL_SIMPLE:
            {
                SerializableObject* ret = entry->e_simple(reader);
                DataObjectSkipEnd(reader);
                READER_MOVE_BYTES(reader, sizeof(saved_obj_id_t),
                    {last_deserialize_error = DESERR_BOUNDS; return nullptr;})
                return ret;
            }

        default:
            reader.data = reader_restore;
            last_deserialize_error = (entry->type == DESER_TBL_NONE) ? DESERR_NOCLASS : DESERR_INVTYPE;
            return nullptr;
    }
}

int deserializeInplace(SerializableObject* object, DataStorageReader& reader, DeserTblEntry* entry, void* reader_restore) {
    if (!entry) {
        reader.data = reader_restore;
        return DESERR_NOCLASS;
    }

    switch (entry->type){
        case DESER_TBL_INPLACE:
            {
                int ret = entry->e_inplace(object, reader);
                if (ret) {
                    reader.data = reader_restore;
                    return ret;
                }
                DataObjectSkipEnd(reader);
                READER_MOVE_BYTES(reader, sizeof(saved_obj_id_t), return DESERR_BOUNDS;)
                return ret;
            }
        default:
            reader.data = reader_restore;
            return (entry->type == DESER_TBL_NONE) ? DESERR_NOCLASS : DESERR_INVTYPE;
    }
}

int deserializeInplaceFull(SerializableObject* object, DataStorageReader& reader) {
    void* reader_restore = reader.data;
    DeserTblEntry* entry = getDeserEntry(reader, reader_restore);
    return deserializeInplace(object, reader, entry, reader_restore);
}

PhysicalBody* physicalBodyDeserialize(DataStorageReader &reader) {
    READER_CHECK_BYTES(reader, sizeof(saved_obj_id_t), {last_deserialize_error = DESERR_BOUNDS; return nullptr;})
    saved_obj_id_t obj_class = *(saved_obj_id_t*)(reader.data);

    if (obj_class != SAVED_OBJ_PHYSICALBODY){
        return nullptr;
    }

    void* reader_restore = reader.data;
    DeserTblEntry* entry = getDeserEntry(reader, reader_restore);
    if (entry == (obj_deser_tbl + SAVED_OBJ_PALLETE_ITEM)) {
        obj_fixed_data_len_t len = *(obj_fixed_data_len_t*)(reader.data);
        PTR_MOVE_BYTES(reader.data, len);
        PhysicalBody* ret = physicalBodyDeserialize(reader);
        if (!ret) {
            reader.data = reader_restore;
            return nullptr;
        }
        DataObjectSkipEnd(reader);
        READER_MOVE_BYTES(reader, sizeof(saved_obj_id_t),
                    {last_deserialize_error = DESERR_BOUNDS; return nullptr;})
        return ret;
    }
    last_deserialize_error = DESERR_INVTYPE;
    return dynamic_cast<PhysicalBody*>(deserializeSimple(reader, entry, reader_restore));
}

int palleteItemDeserialize(PalleteItem* item, DataStorageReader &reader) {
    READER_CHECK_BYTES(reader, sizeof(saved_obj_id_t), return DESERR_BOUNDS;)
    saved_obj_id_t obj_class = *(saved_obj_id_t*)(reader.data);
    if (obj_class == SAVED_OBJ_PHYSICALBODY) {
        PhysicalBody* body = physicalBodyDeserialize(reader);
        if (!body) {
            return last_deserialize_error;
        }
        if (item->body) {
            delete item->body;
        }
        item->body = body;
        item->name = "*Unnamed*";
        return DESERR_OK;
    }

    void* reader_restore = reader.data;
    DeserTblEntry* entry = getDeserEntry(reader, reader_restore);

    return deserializeInplace(item, reader, entry, reader_restore);
}

int palleteDeserialize(SerializableObject* obj, DataStorageReader &reader) {
    Pallete* pallete = static_cast<Pallete*>(obj);

    READER_CHECK_BYTES(reader, sizeof(saved_obj_id_t), return DESERR_BOUNDS;)
    saved_obj_id_t obj_class = *(saved_obj_id_t*)(reader.data);
    if (obj_class == SAVED_OBJ_PKG) {
        size_t size = 0;
        int ret = readPkgHeader(reader, &size);
        if (ret)
            return ret;

        pallete->Clear();
        for (unsigned i = 0; i < size; i++) {
            PalleteItem* item = pallete->AddPalleteItem(nullptr, nullptr);
            ret = palleteItemDeserialize(item, reader);
            if (ret) {
                return ret;
            }
        }
        return DESERR_OK;
    }
    pallete->Clear();
    PalleteItem* item = pallete->AddPalleteItem(nullptr, nullptr);
    int ret = palleteItemDeserialize(item, reader);
    if (ret) {
        pallete->RemovePalleteItem(item);
        return ret;
    }
    return DESERR_OK;
}
