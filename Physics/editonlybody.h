#ifndef EDITONLYBODY_H
#define EDITONLYBODY_H

#include "physicalbody.h"

class EditOnlyBody : public PhysicalBody
{
private:
    QVector2D origin;
    QVector2D velocity;
    double radius;
    DrawingStyle style;
public:
    EditOnlyBody(const QPoint &origin, double radius, DrawingStyle style);

    //general methods
public:
    QRectF GetBoundingRect() const;
    void WidenInspectorContext();
    bool ContainsPoint(const QPoint &point) const;
    PhysicalBody *Clone() const;

    //simulation methods
public:
    void SolveCollision(PhysicalBody *another);
    void KeepSceneBorders(const QRect &world_rect);
    void ApplyInternalRestrictions(double delta_time);
    void ApplyGravity(double air_density, double g, double delta_time);
    void MoveBy(const QPoint &offset);
    void AddMomentum(const QVector2D &momentum);

    //drawing methods
public:
    void Draw(QPainter &painter);

    //debugging methods
public:
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const;
    QVector2D GetCenterVelocity() const override {return this->velocity;}
};

#endif // EDITONLYBODY_H
