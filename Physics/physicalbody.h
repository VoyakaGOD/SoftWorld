#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>
#include <QPainter>
#include "drawingstyle.h"
#include <Utils/fileworks.h>
#include "Inspector/inspector.h"

enum BodyClass {
    BODY_CLASS_UNDEF = 0,
    BODY_CLASS_GHOST = 0x77,
};

class PhysicalBody
{
//general methods
public:
    PhysicalBody() {};

    virtual QRect GetBoundingRect() const = 0;
    virtual void WidenInspectorContext() = 0;
    virtual bool ContainsPoint(const QPoint &point) const = 0;
    virtual PhysicalBody *Clone() const = 0;

//fileworks
public:
    virtual size_t GetSavedSize() const;

    PhysicalBody(DataStorageReader &data);

    virtual void SaveID(DataStorageWriter &data) const;

    virtual void SaveData(DataStorageWriter &data) const;



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

public:
    DrawingStyle drawing_style;
};

#endif // PHYSICALBODY_H
