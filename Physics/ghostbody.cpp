#include "ghostbody.h"
#include <Serialize/serialize.h>
#include <Serialize/deserialize.h>
#include <iostream> //TEMPORARY

struct GhostBodyData {
    obj_fixed_data_len_t size;
    int x, y;
    float radius;
    DrawingStyle drawing_style;
};

static GhostBodyData default_data = {.x = 10, .y = 10, .radius = 10, .drawing_style = DrawingStyle()};

#define GBDATA(reader, field) GETDATA(reader, GhostBodyData, field)

GhostBody::GhostBody(DataStorageReader &reader) :
    PhysicalBody(reader), origin(GBDATA(reader,x), GBDATA(reader,y)),radius(GBDATA(reader, radius)), drawing_style(GBDATA(reader, drawing_style))  {
    PTR_MOVE_BYTES(reader.data, ((GhostBodyData*)(reader.data))->size);
}

size_t GhostBody::GetSavedSize() const {

    return PhysicalBody::GetSavedSize()
     + sizeof(saved_obj_id_t) + sizeof(GhostBodyData) + 1;
};

void GhostBody::SaveData(DataStorageWriter &data) const {
    PhysicalBody::SaveData(data);

    GhostBodyData* gdata = (GhostBodyData*)(data.data);
    WRITER_CHECK_BYTES(data, sizeof(*gdata))

    gdata->size = sizeof(*gdata);
    gdata->x = this->origin.x();
    gdata->y = this->origin.y();
    gdata->radius = this->radius;
    gdata->drawing_style = this->drawing_style;
    PUT_FIXEDONLY_NUL(data.data)
}

QRectF GhostBody::GetBoundingRect() const {
    return QRectF(this->origin.x() - this->radius,
                 this->origin.y() - this->radius,
                 2 * this->radius,
                 2 * this->radius);
}

void GhostBody::WidenInspectorContext() {
    Inspector::AddHeader("ghost body", LARGE_HEADER);
    Inspector::AddParam("radius", radius, (float)10, (float)250);
    this->drawing_style.WidenInspectorContext();
}

bool GhostBody::ContainsPoint(const QPoint &point) const {
    return (QVector2D(point) - QVector2D(this->origin)).lengthSquared() < this->radius * this->radius;
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

void GhostBody::AddMomentum(const QVector2D &momentum) {
    return;
}

void GhostBody::Draw(QPainter &painter) {
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(this->drawing_style.main_color);
    double border_thickness = min(drawing_style.border_thickness, (double)radius);
    painter.setPen(QPen(this->drawing_style.border_color, border_thickness));
    painter.drawEllipse(this->origin, (int)(this->radius), (int)(this->radius));
    return;
}

QPoint GhostBody::GetLocalCoordinate(const QPoint &global_coordinate) const {
    return global_coordinate - this->origin;
}

QPoint GhostBody::GetGlobalCoordinate(const QPoint &local_coordinate) const {
    return local_coordinate + this->origin;
}
