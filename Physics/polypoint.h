#ifndef POLYPOINT_H
#define POLYPOINT_H

#include <QVector2D>

struct PolyPoint
{
    QVector2D position;
    QVector2D velocity;
    PolyPoint(QVector2D position, QVector2D velocity);
};

#endif // POLYPOINT_H
