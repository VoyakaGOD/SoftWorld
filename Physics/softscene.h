#ifndef SOFTSCENE_H
#define SOFTSCENE_H

#include "physicalbody.h"
#include "../Inspector/inspectorparamslist.h"

class SoftScene
{
public:
    void Draw(QPainter &painter) const;
    void NextStep(double delta_time);
    void AddBody(void /*mind*/);
    void RemoveBody(void /*mind*/);
    InspectorParamsList GetInspectorParams() const;
    PhysicalBody &GetBodyAt(const QPoint &point) const;
};

#endif // SOFTSCENE_H
