#pragma once

#include "fileworks.h"
#include "physicalbody.h"

void bodySerialize(PhysicalBody *body, DataStorageWriter &ser);

PhysicalBody* bodyDeserialize(DataStorageReader &deser);
