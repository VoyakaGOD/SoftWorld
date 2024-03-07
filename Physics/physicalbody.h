#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>

class PhysicalBody
{
public:
    QRect &GetBoundingRect() const = 0;
    void SolveCollision(PhysicalBody &another) = 0;
    void KeepSceneBorders(const QRect &world_rect) = 0;
    void ApplyInternalRestrictions(double delta_time) = 0;
    void ApplyGravity(double air_density, double g, double delta_time) = 0;
};

#endif // PHYSICALBODY_H
