#include "testpolybody.h"

TestPolyBody::TestPolyBody(DrawingStyle style) : style(style)
{
    shape.GetPoints().push_back(PolyPoint(QVector2D(100, 100), QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(200, 120), QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(190, 170), QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(100, 200), QVector2D(0, 0)));
}

QRect TestPolyBody::GetBoundingRect() const
{
    return shape.GetBoundingRect().toRect();
}

void TestPolyBody::WidenInspectorContext()
{
    Inspector::AddHeader("test poly body", LARGE_HEADER);
    style.WidenInspectorContext();
}

bool TestPolyBody::ContainsPoint(const QPoint &point) const
{
    return shape.ContainsPoint(QVector2D(point));
}

PhysicalBody *TestPolyBody::Clone() const
{
    return new TestPolyBody(style);
}

void TestPolyBody::SolveCollision(PhysicalBody *another) {}
void TestPolyBody::KeepSceneBorders(const QRect &world_rect) {}
void TestPolyBody::ApplyInternalRestrictions(double delta_time) {}
void TestPolyBody::ApplyGravity(double air_density, double g, double delta_time) {}

void TestPolyBody::MoveBy(const QPoint &offset)
{
    shape.MoveBy(QVector2D(offset));
}

void TestPolyBody::AddMomentum(const QPoint &momentum){}

void TestPolyBody::Draw(QPainter &painter)
{
    shape.Draw(painter, style);
}

QPoint TestPolyBody::GetLocalCoordinate(const QPoint &global_coordinate) const
{
    return global_coordinate - shape.GetConstPoints()[0].position.toPoint();
}

QPoint TestPolyBody::GetGlobalCoordinate(const QPoint &local_coordinate) const
{
    return local_coordinate + shape.GetConstPoints()[0].position.toPoint();
}
