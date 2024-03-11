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
    InspectableParamType type;
};

#define INSPECTABLE(TYPE, NAME)                                                                                                 \
class Inspectable##NAME : public InspectableParam                                                                               \
{                                                                                                                               \
public:                                                                                                                         \
    const char *name;                                                                                                           \
    TYPE &value;                                                                                                                \
    Inspectable##NAME(const char *name, TYPE &value) : name(name), value(value) { this->type = InspectableParamType::NAME; }    \
}

#define INSPECTABLE_NUMERIC(TYPE, NAME, MIN_VALUE, MAX_VALUE)                                                                   \
class Inspectable##NAME : public InspectableParam                                                                               \
{                                                                                                                               \
public:                                                                                                                         \
    const char *name;                                                                                                           \
    TYPE min;                                                                                                                   \
    TYPE max;                                                                                                                   \
    TYPE &value;                                                                                                                \
    Inspectable##NAME(const char *name, TYPE &value) : name(name), value(value), min(MIN_VALUE), max(MAX_VALUE)                 \
    { this->type = InspectableParamType::NAME; }                                                                                \
}

INSPECTABLE_NUMERIC(int, Int, INT_MIN, INT_MAX);
INSPECTABLE_NUMERIC(float, Float, 1e35, -1e35);
INSPECTABLE_NUMERIC(double, Double, 1e305, -1e305);
INSPECTABLE(QColor, Color);

#undef INSPECTABLE

#endif // INSPECTABLEPARAM_H
