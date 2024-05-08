#ifndef IGSHELL_H
#define IGSHELL_H

#include "physicalbody.h"
#include "polygonphysicalshape.h"

class IGShell : public PhysicalBody, public PolygonPhysicalShape
{
private:
    DrawingStyle style;
    double mass;
    double gas_const;
    double shell_bounce;
    double shell_rigidity;
    double current_area;
    double current_density;
    double initial_part_length;
    InspectorLabelManager area_label_manager;
    InspectorLabelManager density_label_manager;
    double reconstruction_radius;
    int reconstruction_detailing;

public:
    IGShell(QVector2D position, double radius, int detailing, DrawingStyle style,
           double mass, double gas_const, double shell_bounce, double shell_rigidity);
    QRectF GetBoundingRect() const;
    void WidenInspectorContext();
    bool ContainsPoint(const QPoint &point) const;
    PhysicalBody *Clone() const;
    void SolveCollision(PhysicalBody *another);
    void KeepSceneBorders(const QRect &world_rect);
    void ApplyInternalRestrictions(double delta_time);
    void ApplyGravity(double air_density, double g, double delta_time);
    void MoveBy(const QPoint &offset);
    void AddMomentum(const QVector2D &momentum);
    void Draw(QPainter &painter);
    QPoint GetLocalCoordinate(const QPoint &global_coordinate) const;
    QPoint GetGlobalCoordinate(const QPoint &local_coordinate) const;
    QVector2D GetCenterVelocity() const override;

private:
    IGShell();
    void Reconstruct(QVector2D position, double radius, int detailing);
};

#endif // IGSHELL_H
