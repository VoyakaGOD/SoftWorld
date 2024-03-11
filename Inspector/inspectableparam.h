#ifndef INSPECTABLEPARAM_H
#define INSPECTABLEPARAM_H

#include <QColor>

enum InspectableParamType
{
    Int,
    Double,
    Float,
    Color
};

class InspectableParam
{
public:
    const char *name;
    InspectableParamType type;
    InspectableParam(const char *name, InspectableParamType type) : name(name), type(type) {}
};

template<typename T>
class CertainInspectableParam : public InspectableParam
{
public:
    T &value;
    T min;
    T max;
    CertainInspectableParam(const char *name, InspectableParamType type, T &value, T min, T max) :
        InspectableParam(name, type), value(value), min(min), max(max) { }
    CertainInspectableParam(const char *name, InspectableParamType type, T &value) :
        InspectableParam(name, type), value(value) { }
};

#endif // INSPECTABLEPARAM_H
