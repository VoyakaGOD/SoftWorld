#include "polypoint.h"

PolyPoint::PolyPoint(const QVector2D &position, const QVector2D &velocity)
{
    this->position = position;
    this->velocity = velocity;
}

void PolyPoint::KeepBorders(const QRect &rect, float bounce)
{
    if(position.x() > rect.right())
    {
        position.setX(rect.right() - 0.05f);
        velocity.setX(-bounce * velocity.x());
    }
    if(position.x() < rect.left())
    {
        position.setX(rect.left() + 0.05f);
        velocity.setX(-bounce * velocity.x());
    }
    if(position.y() < rect.top())
    {
        position.setY(rect.top() + 0.05f);
        velocity.setY(-bounce * velocity.y());
    }
    if(position.y() > rect.bottom())
    {
        position.setY(rect.bottom() - 0.05f);
        velocity.setY(-bounce * velocity.y());
    }
}
