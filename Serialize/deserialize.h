#pragma once

#include "deser_helpers.h"

SerializableObject* deserializeSimple(DataStorageReader &reader, DeserTblEntry* entry, void* reader_restore);
int deserializeInplace(SerializableObject* object, DataStorageReader& reader, DeserTblEntry* entry, void* reader_restore);

#ifdef PHYSICALBODY_H
PhysicalBody* physicalBodyDeserialize(DataStorageReader &deser);
#endif

#ifdef PALLETEITEM_H
int palleteItemDeserialize(PalleteItem* item, DataStorageReader &deser);
#endif

#ifdef PALLETE_H
int palleteDeserialize(Pallete* pallete, DataStorageReader &reader);
#endif

void DataObjectSkipEnd(DataStorageReader &deser);
void DataObjectSkip(DataStorageReader &deser);
