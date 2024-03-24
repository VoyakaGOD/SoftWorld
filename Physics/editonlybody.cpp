#include "editonlybody.h"

EditOnlyBody::EditOnlyBody(const QPoint &origin, double radius, DrawingStyle style) : style(style)
{
    this->origin = origin;
    this->radius = radius;
}

QRect EditOnlyBody::GetBoundingRect() const
{
    return QRect(this->origin.x() - this->radius, this->origin.y() - this->radius,
        this->origin.x() + this->radius, this->origin.y() + this->radius);
}

void EditOnlyBody::WidenInspectorContext()
{
    Inspector::AddHeader("edit-only body", LARGE_HEADER);
    Inspector::AddParam("radius", radius, 0, 250);
    style.WidenInspectorContext();
}

bool EditOnlyBody::ContainsPoint(const QPoint &point) const
{
    return (point - this->origin).manhattanLength() < radius;
}

PhysicalBody *EditOnlyBody::Clone() const
{
    return new EditOnlyBody(origin, radius, style);
}

void EditOnlyBody::SolveCollision(PhysicalBody *another) {}
void EditOnlyBody::KeepSceneBorders(const QRect &world_rect) {}
void EditOnlyBody::ApplyInternalRestrictions(double delta_time) {}
void EditOnlyBody::ApplyGravity(double air_density, double g, double delta_time) {}

void EditOnlyBody::MoveBy(const QPoint &offset)
{
    this->origin = this->origin + offset;
}

void EditOnlyBody::AddMomentum(const QPoint &momentum) {}

void EditOnlyBody::Draw(QPainter &painter)
{
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(style.main_color);
    painter.setPen(QPen(style.border_color, style.border_thickness));
    painter.drawEllipse(this->origin, (int)this->radius, (int)this->radius);
}

QPoint EditOnlyBody::GetLocalCoordinate(const QPoint &global_coordinate) const
{
    return global_coordinate - this->origin;
}

QPoint EditOnlyBody::GetGlobalCoordinate(const QPoint &local_coordinate) const
{
    return local_coordinate + this->origin;
}
