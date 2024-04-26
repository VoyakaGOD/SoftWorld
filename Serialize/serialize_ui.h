#pragma once

#include <QWidget>
#include "deser_helpers.h"

bool guiSerializeObj(QWidget* parent, SerializableObject &obj);

bool guiDeserializeObjInplace(QWidget* parent, SerializableObject *obj
        , int (*func)(SerializableObject*, DataStorageReader&));
