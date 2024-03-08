#include "physicalbody.h"

class GhostBody: public PhysicalBody {

public:
    GhostBody() : origin(0, 0), radius(10) {
    }

    QPoint origin;
    float radius;

public:
    virtual QRect GetBoundingRect() const override;
    virtual InspectableParamsList GetInspectableParams() override;
    virtual bool ContainsPoint(const QPoint &point) const override;
    virtual PhysicalBody *Clone() override;

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
    void DrawBy(QPainter &painter);

//debugging methods
public:
    QPoint GetLocalCoordinate(QPoint &global_coordinate) const override;
    QPoint GetGlobalCoordinate(QPoint &local_coordinate) const override;

};
