#pragma once

#include "physicalbody.h"

///SERIALIZABLE GhostBody:PhysicalBody GHOST_BODY_ID simple

class GhostBody: public PhysicalBody {

public:
    GhostBody() : origin(10, 10), radius(10) {
    }

    DrawingStyle drawing_style;
    QPoint origin;
    float radius;

// fileworks
public:
    virtual void SaveID(DataStorageWriter &data) const override;
    virtual size_t GetSavedSize() const override;
    virtual void SaveData(DataStorageWriter &data) const override;
    static SerializableObject* Deserialize(DataStorageReader& reader) {return new GhostBody(reader);}
    GhostBody(DataStorageReader &reader);

public:

    virtual QRectF GetBoundingRect() const override;
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
    virtual void AddMomentum(const QVector2D &momentum) override;

//drawing methods
public:
    void Draw(QPainter &painter);

//debugging methods
public:
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const override;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const override;

};
