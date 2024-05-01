#ifndef POLYGONPHYSICALSHAPE_H
#define POLYGONPHYSICALSHAPE_H

#include <vector>
#include <QRectF>
#include <QPainter>
#include "polypoint.h"
#include "Utils/drawingstyle.h"

using namespace std;

class PolygonPhysicalShape
{
private:
    vector<PolyPoint> points;
    mutable vector<QPoint> drawing_points;

public:
    PolygonPhysicalShape();
    vector<PolyPoint> &GetPoints();
    const vector<PolyPoint> &GetConstPoints() const;
    QRectF GetBoundingRect() const;
    bool ContainsPoint(const QVector2D &point) const;
    void GetSideBySideIntersectionPoints(const PolygonPhysicalShape &another, vector<QVector2D> &points) const;
    void Draw(QPainter &painter, const DrawingStyle &style) const;
    void UpdatePositions(double delta_time);
    void MoveBy(const QVector2D &offset);
    void AddVelocity(const QVector2D &delta_velocity);
};

#endif // POLYGONPHYSICALSHAPE_H
