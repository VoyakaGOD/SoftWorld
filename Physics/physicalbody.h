#ifndef PHYSICALBODY_H
#define PHYSICALBODY_H

#include <QPoint>
#include <QRect>

class PhysicalBody
{
public:
    const QRect &GetBoundingRect() const = 0;
    void SolveCollision(PhysicalBody &another) = 0;
    void KeepSceneBorders(const QRect &worlRect) = 0;
};

#endif // PHYSICALBODY_H
