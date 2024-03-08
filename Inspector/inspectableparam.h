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

INSPECTABLE(int, Int);
INSPECTABLE(double, Double);
INSPECTABLE(float, Float);
INSPECTABLE(QColor, Color);

#undef INSPECTABLE

#endif // INSPECTABLEPARAM_H
