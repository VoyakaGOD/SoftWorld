#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>
#include <QPainter>
#include "drawingstyle.h"
#include "../Inspector/inspectableparamslist.h"

class PhysicalBody
{
//general methods
public:
    virtual QRect GetBoundingRect() const = 0;
    virtual InspectableParamsList GetInspectableParams() = 0;
    virtual bool ContainsPoint(const QPoint &point) const = 0;
    virtual PhysicalBody *Clone() = 0;

//simulation methods
public:
    virtual void SolveCollision(PhysicalBody *another) = 0;
    virtual void KeepSceneBorders(const QRect &world_rect) = 0;
    virtual void ApplyInternalRestrictions(double delta_time) = 0;
    virtual void ApplyGravity(double air_density, double g, double delta_time) = 0;
    virtual void MoveTo(const QPoint &point) = 0;
    virtual void AddMomentum(const QPoint &momentum) = 0;

//drawing methods
public:
    virtual void DrawBy(QPainter &painter) const = 0;

//debugging methods
public:
    virtual QPoint GetLocalCoordinate(QPoint &&global_coordinate) const = 0;
    virtual QPoint GetGlobalCoordinate(QPoint &&local_coordinate) const = 0;
};

#endif // PHYSICALBODY_H
