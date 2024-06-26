#include "editonlybody.h"
#include <Serialize/ser_class_enums.h>

struct EditOnlyBodyData{
    obj_fixed_data_len_t size;
    QVector2D origin;
    QVector2D velocity;
    double radius;
    DrawingStyle style;
    double bounce;
};

EditOnlyBodyData default_data {
    .origin = QVector2D(0,0),
    .velocity  = QVector2D(0,0),
    .radius = 10,
    .style = DrawingStyle(),
    .bounce = 0.5
};

EditOnlyBody::EditOnlyBody(const QPoint &origin, double radius, DrawingStyle style) : style(style)
{
    this->origin = QVector2D(origin);
    velocity = QVector2D(100, -250);
    this->radius = radius;
    bounce = 0.7;
}

EditOnlyBody::EditOnlyBody(DataStorageReader &reader) :
        PhysicalBody(reader),
        origin(GETDATA(reader, EditOnlyBodyData, origin)), velocity(GETDATA(reader, EditOnlyBodyData, velocity)),
        radius(GETDATA(reader, EditOnlyBodyData, radius)), style(GETDATA(reader, EditOnlyBodyData, style)),
        bounce(GETDATA(reader, EditOnlyBodyData, bounce)) {
    PTR_MOVE_BYTES(reader.data, ((EditOnlyBodyData*)(reader.data))->size);
}

size_t EditOnlyBody::GetSavedSize() const {
    return PhysicalBody::GetSavedSize()
     + sizeof(saved_obj_id_t) + sizeof(EditOnlyBodyData) + 1;
}

void EditOnlyBody::SaveData(DataStorageWriter &writer) const {
    PhysicalBody::SaveData(writer);

    EditOnlyBodyData* data = (EditOnlyBodyData*)(writer.data);
    WRITER_MOVE_BYTES(writer, sizeof(*data))

    data->size = sizeof(*data);
    data->origin = this->origin;
    data->velocity = this->velocity;
    data->radius = this->radius;
    data->bounce = this->bounce;
    data->style = this->style;

    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_NONE)
}


QRectF EditOnlyBody::GetBoundingRect() const
{
    return QRectF(this->origin.x() - this->radius, this->origin.y() - this->radius,
        2*this->radius, 2*this->radius);
}

void EditOnlyBody::WidenInspectorContext()
{
    Inspector::AddHeader("edit-only body", LARGE_HEADER);
    Inspector::AddParam("radius", radius, 15, 250);
    Inspector::AddParam("bounce", bounce, 0, 0.95);
    style.WidenInspectorContext();
}

bool EditOnlyBody::ContainsPoint(const QPoint &point) const
{
    return (QVector2D(point) - origin).lengthSquared() < radius * radius;
}

PhysicalBody *EditOnlyBody::Clone() const
{
    return new EditOnlyBody(origin.toPoint(), radius, style);
}

void EditOnlyBody::SolveCollision(PhysicalBody *another) {}

void EditOnlyBody::KeepSceneBorders(const QRect &world_rect)
{
    if(origin.x() + radius > world_rect.right())
    {
        origin.setX(world_rect.right() - radius);
        velocity.setX(-bounce * velocity.x());
    }
    if(origin.x() - radius < world_rect.left())
    {
        origin.setX(world_rect.left() + radius);
        velocity.setX(-bounce * velocity.x());
    }
    if(origin.y() - radius < world_rect.top())
    {
        origin.setY(world_rect.top() + radius);
        velocity.setY(-bounce * velocity.y());
    }
    if(origin.y() + radius > world_rect.bottom())
    {
        origin.setY(world_rect.bottom() - radius);
        velocity.setY(-bounce * velocity.y());
    }
}

void EditOnlyBody::ApplyInternalRestrictions(double delta_time)
{
    origin += velocity * delta_time;
}

void EditOnlyBody::ApplyGravity(double air_density, double g, double delta_time)
{
    velocity += QVector2D(0, g * delta_time);
}

void EditOnlyBody::MoveBy(const QPoint &offset)
{
    this->origin = this->origin + QVector2D(offset);
    //velocity = QVector2D(0, 0);
}

void EditOnlyBody::AddMomentum(const QVector2D &momentum) {
    this->velocity += QVector2D(momentum);
}

void EditOnlyBody::Draw(QPainter &painter)
{
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(style.main_color);
    double border_thickness = min(style.border_thickness, radius);
    painter.setPen(QPen(style.border_color, border_thickness));
    painter.drawEllipse(this->origin.toPoint(), (int)this->radius, (int)this->radius);
}

QPoint EditOnlyBody::GetLocalCoordinate(const QPoint &global_coordinate) const
{
    return global_coordinate - this->origin.toPoint();
}

QPoint EditOnlyBody::GetGlobalCoordinate(const QPoint &local_coordinate) const
{
    return local_coordinate + this->origin.toPoint();
}
