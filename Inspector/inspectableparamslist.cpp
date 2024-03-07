#include "inspectableparamslist.h"

InspectableParamsList::InspectableParamsList(const char *target_name, size_t capacity) : target_name(target_name)
{
    params.reserve(capacity);
}
