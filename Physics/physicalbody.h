#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>
#include <QPainter>
#include "drawingstyle.h"
#include "Inspector/inspectableparam.h"

class PhysicalBody
{
//general methods
public:
    virtual QRect GetBoundingRect() const = 0;
    virtual InspectableParamsList GetInspectableParams() = 0;
    virtual bool ContainsPoint(const QPoint &point) const = 0;
    virtual PhysicalBody *Clone() const = 0;

//simulation methods
public:
    virtual void SolveCollision(PhysicalBody *another) = 0;
    virtual void KeepSceneBorders(const QRect &world_rect) = 0;
    virtual void ApplyInternalRestrictions(double delta_time) = 0;
    virtual void ApplyGravity(double air_density, double g, double delta_time) = 0;
    virtual void MoveBy(const QPoint &offset) = 0;
    virtual void AddMomentum(const QPoint &momentum) = 0;

//drawing methods
public:
    virtual void Draw(QPainter &painter) = 0;

//debugging methods
public:
    virtual QPoint GetLocalCoordinate(const QPoint &global_coordinate) const = 0;
    virtual QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const = 0;
};

#endif // PHYSICALBODY_H
