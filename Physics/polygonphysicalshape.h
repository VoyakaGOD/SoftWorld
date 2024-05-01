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

public:
    PolygonPhysicalShape();
    vector<PolyPoint> &GetPoints();
    QRectF GetBoundingRect() const;
    bool ContainsPoint(QVector2D point) const;
    void GetSideBySideIntersectionPoints(const PolygonPhysicalShape &another, vector<QVector2D> &points) const;
    void Draw(QPainter &painter, const DrawingStyle &style) const;
    void UpdatePositions(double delta_time);
};

#endif // POLYGONPHYSICALSHAPE_H
