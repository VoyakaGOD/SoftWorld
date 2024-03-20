#pragma once

#include "physicalbody.h"

struct GhostBodyData {
    PhysicalBodyData super;
    int x, y;
    float radius;
};

class GhostBody: public PhysicalBody {

public:
    GhostBody() : origin(10, 10), radius(10) {
    }

    QPoint origin;
    float radius;

// fileworks
public:
    virtual BodyClass GetClass() const override {return BODY_CLASS_GHOST;}
    virtual size_t GetDataSize() const override {return sizeof(GhostBodyData);}
    virtual void GetData(void* data) const override;
    GhostBody(GhostBodyData* data) : PhysicalBody(&(data->super)), origin(data->x, data->y), radius(data->radius) {printf("D %d %d %d\n", data->x, data->y, data->radius);}



public:

    virtual QRect GetBoundingRect() const override;
    virtual void WidenInspectorContext() override;
    virtual bool ContainsPoint(const QPoint &point) const override;
    virtual PhysicalBody *Clone() const override;

//simulation methods
public:

    virtual void SolveCollision(PhysicalBody *another) override;
    virtual void KeepSceneBorders(const QRect &world_rect) override;
    virtual void ApplyInternalRestrictions(double delta_time) override;
    virtual void ApplyGravity(double air_density, double g, double delta_time) override;
    virtual void MoveBy(const QPoint &offset) override;
    virtual void AddMomentum(const QPoint &momentum) override;

//drawing methods
public:
    void Draw(QPainter &painter);

//debugging methods
public:
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const override;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const override;

};
