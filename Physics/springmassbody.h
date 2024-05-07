#ifndef SPRING_MASS_BODY_H
#define SPRING_MASS_BODY_H

#include "physicalbody.h"
#include "polygonphysicalshape.h"
#include <vector>

///SERIALIZABLE SpringMassBody:PhysicalBody SPRMSS_BODY_ID simple

struct SpringBodyRow {
    int y;
    int startx;
    vector<PolyPoint> atoms;

    PolyPoint& operator[](int i) {
        return this->atoms[i];
    }

    size_t size() const{
        return this->atoms.size();
    }
};

class SpringMassBody : public PhysicalBody, public PolygonPhysicalShape {
    double xscale = 10, yscale = 10, stiffness = 500, resistance = 1, density = 1;
    double diagscale = sqrt((xscale * xscale) + (yscale * yscale));
    vector<SpringBodyRow> rows;
    DrawingStyle style;

    public:
        SpringMassBody();

    public:
        virtual size_t GetSavedSize() const override;

        SpringMassBody(DataStorageReader &data);
        static SerializableObject* Deserialize(DataStorageReader& reader) {return new SpringMassBody(reader);}

        virtual void SaveID(DataStorageWriter &data) const override;

        virtual void SaveData(DataStorageWriter &data) const override;

    public:

        virtual void WidenInspectorContext() override;
        virtual QRectF GetBoundingRect() const override;
        //virtual void WidenInspectorContext() const override;
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
        virtual void Draw(QPainter &painter) override;

    //debugging methods
    public:
        virtual QPoint GetLocalCoordinate(const QPoint &global_coordinate) const override;
        virtual QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const override;
        virtual QVector2D GetCenterVelocity() const override;

    protected:
        void RecalculatePolyPoints();
};

#endif
