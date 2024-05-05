#ifndef IGBALL_H
#define IGBALL_H

#include "physicalbody.h"
#include "polygonphysicalshape.h"

class IGBall : public PhysicalBody
{
private:
    DrawingStyle style;
    PolygonPhysicalShape shape;
    double mass;
    double gas_const;
    double shell_bounce;
    double shell_rigidity;
    double current_area;
    double current_density;
    InspectorLabelManager area_label_manager;
    InspectorLabelManager density_label_manager;

public:
    IGBall(QVector2D position, double radius, int detailing, DrawingStyle style,
           double mass, double gas_const, double shell_bounce, double shell_rigidity);
    QRect GetBoundingRect() const;
    void WidenInspectorContext();
    bool ContainsPoint(const QPoint &point) const;
    PhysicalBody *Clone() const;
    void SolveCollision(PhysicalBody *another);
    void KeepSceneBorders(const QRect &world_rect);
    void ApplyInternalRestrictions(double delta_time);
    void ApplyGravity(double air_density, double g, double delta_time);
    void MoveBy(const QPoint &offset);
    void AddMomentum(const QPoint &momentum);
    void Draw(QPainter &painter);
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const;

private:
    IGBall();
};

#endif // IGBALL_H
