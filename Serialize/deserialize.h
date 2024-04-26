#pragma once

#include "deser_helpers.h"

SerializableObject* deserializeSimple(DataStorageReader &reader, DeserTblEntry* entry, void* reader_restore);
int deserializeInplace(SerializableObject* object, DataStorageReader& reader, DeserTblEntry* entry, void* reader_restore);

int deserializeInplaceFull(SerializableObject* object, DataStorageReader& reader);

#ifdef PHYSICALBODY_H
PhysicalBody* physicalBodyDeserialize(DataStorageReader &deser);
#endif

#ifdef PALLETEITEM_H
int palleteItemDeserialize(PalleteItem* item, DataStorageReader &deser);
#endif

#ifdef PALLETE_H
int palleteDeserialize(SerializableObject* obj, DataStorageReader &reader); // BADCODE: kostyl
#endif

void DataObjectSkipEnd(DataStorageReader &deser);
void DataObjectSkip(DataStorageReader &deser);
