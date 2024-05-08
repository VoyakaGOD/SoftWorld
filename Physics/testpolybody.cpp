#include "testpolybody.h"
#include <Serialize/ser_class_enums.h>

struct TestPolyBodyData {
    obj_fixed_data_len_t size;
    DrawingStyle style;
    double density;
    float bounce;
};

static TestPolyBodyData default_data = {
    .size = 0, .style = DrawingStyle(), .density = 100, .bounce = 0.5
};

TestPolyBody::TestPolyBody(QVector2D in_pos, DrawingStyle style) : style(style)
{
    shape.GetPoints().push_back(PolyPoint(QVector2D(100, 100) + in_pos, QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(200, 120) + in_pos, QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(190, 170) + in_pos, QVector2D(0, 0)));
    shape.GetPoints().push_back(PolyPoint(QVector2D(100, 200) + in_pos, QVector2D(0, 0)));
    density = 100;
    bounce = 0.5;
}

QRectF TestPolyBody::GetBoundingRect() const
{
    return shape.GetBoundingRect();
}

void TestPolyBody::WidenInspectorContext()
{
    Inspector::AddHeader("test poly body", LARGE_HEADER);
    Inspector::AddParam("density", density, 1.0, 1000.0);
    Inspector::AddParam("bounce", bounce, 0.0f, 0.99f);
    style.WidenInspectorContext();
}

bool TestPolyBody::ContainsPoint(const QPoint &point) const
{
    return shape.ContainsPoint(QVector2D(point));
}

PhysicalBody *TestPolyBody::Clone() const
{
    return new TestPolyBody(QVector2D(0, 0), style);
}

void TestPolyBody::SolveCollision(PhysicalBody *another)
{
    TestPolyBody *tpb = dynamic_cast<TestPolyBody *>(another);
    if(tpb == NULL)
        return;

    vector<QVector2D> int_points;
    shape.GetSideBySideIntersectionPoints(tpb->shape, int_points);
    if(int_points.size() > 0)
        shape.MoveBy(QVector2D(0, 0.5f));
}

void TestPolyBody::KeepSceneBorders(const QRect &world_rect)
{
    for(auto &point : shape.GetPoints())
    {
        point.KeepBorders(world_rect, bounce);
    }
}

void TestPolyBody::ApplyInternalRestrictions(double delta_time)
{
    shape.UpdatePositions(delta_time);
}

void TestPolyBody::ApplyGravity(double air_density, double g, double delta_time)
{
    shape.AddVelocity(QVector2D(0, (1 - air_density / density) * g * delta_time));
}

void TestPolyBody::MoveBy(const QPoint &offset)
{
    shape.MoveBy(QVector2D(offset));
}

void TestPolyBody::AddMomentum(const QVector2D &momentum){}

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

QVector2D TestPolyBody::GetCenterVelocity() const {
    return shape.GetCenterVelocity();
}

size_t TestPolyBody::GetSavedSize() const {
    return PhysicalBody::GetSavedSize() + sizeof(saved_obj_id_t)
     + sizeof(TestPolyBodyData) + shape.GetSavedSize() + sizeof(SAVED_OBJ_NONE);
}

void TestPolyBody::SaveData(DataStorageWriter &writer) const {
    PhysicalBody::SaveData(writer);
    TestPolyBodyData* data = (TestPolyBodyData*)(writer.data);
    WRITER_MOVE_BYTES(writer, sizeof(*data))
    *data = (TestPolyBodyData){.size = sizeof(*data), .style = this->style
                        , .density = this->density, .bounce = this->bounce};
    shape.SaveData(writer);
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_NONE);
}

TestPolyBody::TestPolyBody(DataStorageReader &reader) :
PhysicalBody(reader), style(GETDATA(reader, TestPolyBodyData, style)),
density(GETDATA(reader, TestPolyBodyData, density)), bounce(GETDATA(reader, TestPolyBodyData, bounce)) {

    READER_MOVE_BYTES(reader, ((TestPolyBodyData*)reader.data)->size, {last_deserialize_error = DESERR_BOUNDS; return;})
    shape.Deserialize(reader);
}
