#ifndef TESTPOLYBODY_H
#define TESTPOLYBODY_H

#include "physicalbody.h"
#include "polygonphysicalshape.h"

///SERIALIZABLE TestPolyBody:PhysicalBody TESTPOLY_BODY_ID simple

class TestPolyBody : public PhysicalBody
{
private:
    DrawingStyle style;
    PolygonPhysicalShape shape;
    double density;
    float bounce;

public:
    TestPolyBody(QVector2D in_pos, DrawingStyle style);

// fileworks
public:
    virtual void SaveID(DataStorageWriter &data) const override;
    virtual size_t GetSavedSize() const override;
    virtual void SaveData(DataStorageWriter &data) const override;
    static SerializableObject* Deserialize(DataStorageReader& reader) {return new TestPolyBody(reader);}
    TestPolyBody(DataStorageReader &writer);

    //general methods
public:
    QRectF GetBoundingRect() const;
    void WidenInspectorContext();
    bool ContainsPoint(const QPoint &point) const;
    PhysicalBody *Clone() const;

    //simulation methods
public:
    void SolveCollision(PhysicalBody *another);
    void KeepSceneBorders(const QRect &world_rect);
    void ApplyInternalRestrictions(double delta_time);
    void ApplyGravity(double air_density, double g, double delta_time);
    void MoveBy(const QPoint &offset);
    void AddMomentum(const QVector2D &momentum);

    //drawing methods
public:
    void Draw(QPainter &painter);

    //debugging methods
public:
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const;
    QVector2D GetCenterVelocity() const override;
};

#endif // TESTPOLYBODY_H
