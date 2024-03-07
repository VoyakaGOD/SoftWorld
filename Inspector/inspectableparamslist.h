#ifndef INSPECTABLEPARAMSLIST_H
#define INSPECTABLEPARAMSLIST_H

#include <vector>
#include "inspectableparam.h"

using namespace std;

class InspectableParamsList
{
public:
    const char *target_name;
    vector<InspectableParam> params;
    InspectableParamsList(const char *target_name, size_t capacity);
};

#endif // INSPECTABLEPARAMSLIST_H
