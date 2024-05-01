#include "polygonphysicalshape.h"

PolygonPhysicalShape::PolygonPhysicalShape() : points() {}

vector<PolyPoint> &PolygonPhysicalShape::GetPoints()
{
    return points;
}

const vector<PolyPoint> &PolygonPhysicalShape::GetConstPoints() const
{
    return points;
}

QRectF PolygonPhysicalShape::GetBoundingRect() const
{
    if(points.size() == 0)
        return QRectF();

    float left = points[0].position.x();
    float right = points[0].position.x();
    float top = points[0].position.y();
    float bottom = points[0].position.y();

    for(auto &point : points)
    {
        left = min(left, point.position.x());
        right = max(right, point.position.x());
        top = min(top, point.position.y());
        bottom = max(bottom, point.position.y());
    }

    return QRectF(left, top, right - left, bottom - top);
}

static bool CheckUppperRaycast(QVector2D origin, QVector2D p1, QVector2D p2)
{
    if((origin.x() <= min(p1.x(), p2.x())) || (origin.x() >= max(p1.x(), p2.x())))
        return false;

    QVector2D a = p2 - p1;
    double k = a.y() / a.x();
    double proj_y = p1.y() + k * (origin.x() - p1.x());

    return proj_y < origin.y();
}

bool PolygonPhysicalShape::ContainsPoint(const QVector2D &point) const
{
    if(points.size() < 3)
        return false;

    bool result = false;

    result ^= CheckUppperRaycast(point, points[0].position, points[points.size() - 1].position);
    for(int i = 1; i < points.size(); i++)
        result ^= CheckUppperRaycast(point, points[i - 1].position, points[i].position);

    return result;
}

void PolygonPhysicalShape::GetSideBySideIntersectionPoints(const PolygonPhysicalShape &another, vector<QVector2D> &points) const
{
    return;
}

void PolygonPhysicalShape::Draw(QPainter &painter, const DrawingStyle &style) const
{
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(style.main_color);
    QRectF bounding_rect = GetBoundingRect();
    double border_thickness = min(style.border_thickness, min(bounding_rect.width(), bounding_rect.height()));
    painter.setPen(QPen(style.border_color, border_thickness));

    while(drawing_points.size() < points.size())
        drawing_points.push_back(QPoint());

    for(int i = 0; i < points.size(); i++)
        drawing_points[i] = points[i].position.toPoint();
    painter.drawPolygon(drawing_points.data(), points.size());
}

void PolygonPhysicalShape::UpdatePositions(double delta_time)
{
    for(auto &point : points)
        point.position += point.velocity * delta_time;
}

void PolygonPhysicalShape::MoveBy(const QVector2D &offset)
{
    for(auto &point : points)
        point.position += offset;
}
