#include "inspectorcontext.h"

#define ADD_CLIPPED_PARAM(TYPE, P_TYPE)                                                                         \
void InspectorContext::AddParam(const char *name, TYPE &value, TYPE min, TYPE max)                              \
{                                                                                                               \
        params.push_back(CertainInspectableParam(name, P_TYPE, value, min, max));                               \
}

#define ADD_PARAM(TYPE, P_TYPE)                                                                                 \
void InspectorContext::AddParam(const char *name, TYPE &value)                                                  \
{                                                                                                               \
    params.push_back(CertainInspectableParam(name, P_TYPE, value));                                             \
}

InspectorContext::InspectorContext(const char *target_name, size_t params_capacity, size_t actions_capacity) : target_name(target_name)
{
    params.reserve(params_capacity);
    actions.reserve(actions_capacity);
}

void InspectorContext::SetTargetName(const char *name)
{
    target_name = name;
}

void InspectorContext::AddAction(const char *name, Action action)
{
    actions.push_back(InspectableAction(name, action));
}

ADD_CLIPPED_PARAM(int, InspectableParamType::Int)
ADD_CLIPPED_PARAM(double, InspectableParamType::Double)
ADD_CLIPPED_PARAM(float, InspectableParamType::Float)
ADD_PARAM(QColor, InspectableParamType::Color)

#undef ADD_CLIPPED_PARAM
#undef ADD_PARAM
