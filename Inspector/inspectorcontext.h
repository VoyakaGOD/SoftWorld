#ifndef INSPECTORCONTEXT_H
#define INSPECTORCONTEXT_H

#include <vector>
#include "inspectableparam.h"
#include "inspectableaction.h"

#define ADD_CLIPPED_PARAM(TYPE, MIN_VALUE, MAX_VALUE) \
void AddParam(const char *name, TYPE &value, TYPE min = MIN_VALUE, TYPE max = MAX_VALUE)

#define ADD_PARAM(TYPE) \
void AddParam(const char *name, TYPE &value)

using namespace std;

class InspectorContext
{
public:
    const char *target_name;
    vector<InspectableParam> params;
    vector<InspectableAction> actions;
    InspectorContext(const char *target_name, size_t params_capacity = 0, size_t actions_capacity = 0);
    void SetTargetName(const char *name);
    void AddAction(const char *name, Action action);

    ADD_CLIPPED_PARAM(int, INT_MIN, INT_MAX);
    ADD_CLIPPED_PARAM(float, -1e35, 1e35);
    ADD_CLIPPED_PARAM(double, -1e305, 1e305);
    ADD_PARAM(QColor);
};

#undef ADD_CLIPPED_PARAM
#undef ADD_PARAM

#endif // INSPECTORCONTEXT_H
