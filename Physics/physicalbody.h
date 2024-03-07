#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>
#include <QPainter>
#include "drawingstyle.h"
#include "../Inspector/inspectorparamslist.h"

class PhysicalBody
{
//general methods
public:
    QRect &GetBoundingRect() const = 0;
    InspectorParamsList GetInspectorParams() = 0;
    bool ContainsPoint(QPoint &point) const = 0;
    //clone

//simulation methods
public:
    void SolveCollision(PhysicalBody &another) = 0;
    void KeepSceneBorders(const QRect &world_rect) = 0;
    void ApplyInternalRestrictions(double delta_time) = 0;
    void ApplyGravity(double air_density, double g, double delta_time) = 0;
    //MoveTo
    //ApplyForceAt

//drawing methods
public:
    void DrawBy(QPainter &painter) const = 0;

//debugging methods
public:
    QPoint &GetLocalCoordinate(QPoint &global_coordinate) = 0;
    QPoint &GetGlobalCoordinate(QPoint &local_coordinate) = 0;
};

#endif // PHYSICALBODY_H
