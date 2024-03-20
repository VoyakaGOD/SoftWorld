#include "ghostbody.h"

void GhostBody::GetData(void* data) const {
    GhostBodyData* gdata = (GhostBodyData*)data;
    PhysicalBody::GetData(&(gdata->super));
    gdata->x = this->origin.x();
    gdata->y = this->origin.y();
    gdata->radius = this->radius;
}

QRect GhostBody::GetBoundingRect() const {
    return QRect(this->origin.x() - this->radius,
                 this->origin.y() - this->radius,
                 2 * this->radius,
                 2 * this->radius);
}

void GhostBody::WidenInspectorContext() {
    Inspector::AddHeader("ghost body", LARGE_HEADER);
    Inspector::AddParam("radius", radius, (float)10, (float)250);
}

bool GhostBody::ContainsPoint(const QPoint &point) const {
    return (point - this->origin).manhattanLength() < radius;
}

PhysicalBody* GhostBody::Clone() const {
    return new GhostBody(*this);
}

void GhostBody::SolveCollision(PhysicalBody *another) {
    return;
}

void GhostBody::ApplyGravity(double air_density, double g, double delta_time) {
    return;
}

void GhostBody::KeepSceneBorders(const QRect &world_rect) {
    return;
}

void GhostBody::ApplyInternalRestrictions(double delta_time) {
    return;
}

void GhostBody::MoveBy(const QPoint &offset) {
    this->origin = this->origin + offset;
    return;
}

void GhostBody::AddMomentum(const QPoint &momentum) {
    return;
}

void GhostBody::Draw(QPainter &painter) {
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(this->drawing_style.main_color);
    painter.setPen(this->drawing_style.border_color);
    painter.drawEllipse(this->origin, (int)(this->radius), (int)(this->radius));
    return;
}

QPoint GhostBody::GetLocalCoordinate(const QPoint &global_coordinate) const {
    return global_coordinate - this->origin;
}

QPoint GhostBody::GetGlobalCoordinate(const QPoint &local_coordinate) const {
    return local_coordinate + this->origin;
}
