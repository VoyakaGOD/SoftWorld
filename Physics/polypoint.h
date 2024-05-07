#ifndef POLYPOINT_H
#define POLYPOINT_H

#include <QVector2D>

struct PolyPoint
{
    QVector2D position;
    QVector2D velocity;
    PolyPoint();
    PolyPoint(const QVector2D &position, const QVector2D &velocity);

    //bounce should be in [0,1]
    void KeepBorders(const QRect &rect, float bounce);
};

#endif // POLYPOINT_H
