#include "desertab.h"
#include <Physics/physicalbody.h>
#include <UI/palleteitem.h>
#include <UI/pallete.h>
#include "deserialize.h"

DeserializeError last_deserialize_error;

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
        PTR_MOVE_BYTES(deser.data, len);
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

DeserTblEntry* getDeserEntry(DataStorageReader &reader, void* reader_restore) {
    DeserTblEntry* table = obj_deser_tbl;
    DeserTblBaseEntry* base = obj_deser_tbl_b;
    uint8_t tabl_size = obj_deser_tbl_size;
    uint8_t tabl_bsize = obj_deser_tbl_size_b;
    saved_obj_id_t obj_class;

    while (true) {

        PTR_READVAL(reader.data, saved_obj_id_t, obj_class);
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
                PTR_MOVE_BYTES(reader.data, sizeof(saved_obj_id_t));
                return ret;
            }
        default:
            reader.data = reader_restore;
            last_deserialize_error = DESERR_INVTYPE;
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
                DataObjectSkipEnd(reader);
                PTR_MOVE_BYTES(reader.data, sizeof(saved_obj_id_t));
                return ret;
            }
        default:
            reader.data = reader_restore;
            return DESERR_INVTYPE;
    }
}

PhysicalBody* physicalBodyDeserialize(DataStorageReader &reader) {
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
        PTR_MOVE_BYTES(reader.data, sizeof(saved_obj_id_t));
        return ret;
    }
    last_deserialize_error = DESERR_INVTYPE;
    return dynamic_cast<PhysicalBody*>(deserializeSimple(reader, entry, reader_restore));
}

int palleteItemDeserialize(PalleteItem* item, DataStorageReader &reader) {
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

int palleteDeserialize(Pallete* pallete, DataStorageReader &reader) {
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
