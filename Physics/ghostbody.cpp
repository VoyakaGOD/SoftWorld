#include "ghostbody.h"

QRect GhostBody::GetBoundingRect() const {
    return QRect(this->origin.x() - this->radius,
                 this->origin.y() - this->radius,
                 this->origin.x() + this->radius,
                 this->origin.y() + this->radius);
}

InspectableParamsList GhostBody::GetInspectableParams() {
    InspectableParamsList list("ghost", 1);
    list.params.push_back(InspectableFloat("radius", radius));
    return list;
}

bool GhostBody::ContainsPoint(const QPoint &point) const {
    return (point - this->origin).manhattanLength() < radius;
}

PhysicalBody* GhostBody::Clone() const {
    return nullptr;
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
    painter.setBrush(Qt::green);
    painter.setPen(Qt::black);
    painter.drawEllipse(this->origin, (int)(this->radius), (int)(this->radius));
    return;
}

QPoint GhostBody::GetLocalCoordinate(const QPoint &global_coordinate) const {
    return global_coordinate - this->origin;
}

QPoint GhostBody::GetGlobalCoordinate(const QPoint &local_coordinate) const {
    return local_coordinate + this->origin;
}
