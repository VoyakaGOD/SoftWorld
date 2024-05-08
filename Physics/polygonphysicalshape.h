#ifndef POLYGONPHYSICALSHAPE_H
#define POLYGONPHYSICALSHAPE_H

#include <vector>
#include <QRectF>
#include <QPainter>
#include <Utils/drawingstyle.h>
#include <Utils/fileworks.h>
#include "linesintersectioninfo.h"

using namespace std;

class PolygonPhysicalShape
{
protected:
    vector<PolyPoint> points;
    vector<QLine> debuglines;
private:
    mutable vector<QPoint> drawing_points;

public:
    PolygonPhysicalShape();
    vector<PolyPoint> &GetPoints();
    const vector<PolyPoint> &GetConstPoints() const;
    QRectF GetBoundingRect() const;
    bool ContainsPoint(const QVector2D &point) const;
    void GetSideBySideIntersectionPoints(const PolygonPhysicalShape &another, vector<QVector2D> &points) const;
    void GetIntersectionWithLine(QVector2D s, QVector2D e, vector<QVector2D> &points) const;
    void Draw(QPainter &painter, const DrawingStyle &style) const;
    void UpdatePositions(double delta_time);
    void MoveBy(const QVector2D &offset);
    void AddVelocity(const QVector2D &delta_velocity);
    double GetArea() const;
    double getIntersectionArea(PolygonPhysicalShape& other);
    QVector2D GetCenter() const;
    void GetSideBySideIntersectionInfo(PolygonPhysicalShape &another, vector<LinesIntersectionInfo> &info);
    void GetSelfIntersectionInfo(vector<LinesIntersectionInfo> &info);
    void LimitVelocity(double limit);
    QVector2D GetCenterVelocity() const;

    size_t GetSavedSize() const;
    void SaveData(DataStorageWriter& writer) const;
    void Deserialize(DataStorageReader& reader);

};

#endif // POLYGONPHYSICALSHAPE_H
