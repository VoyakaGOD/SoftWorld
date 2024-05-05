#include "igball.h"

static QString get_label_string(double value)
{
    return (value > 0) ? QString::number(value) : "...";
}

IGBall::IGBall(QVector2D position, double radius, int detailing, DrawingStyle style,
    double mass, double gas_const, double shell_bounce, double shell_rigidity)
{
    vector<PolyPoint> &points = shape.GetPoints();
    double dphi = 2 * M_PI / detailing;
    for(int i = 0; i < detailing; i++)
        points.push_back(PolyPoint(position + QVector2D(cos(dphi * i), sin(dphi * i)) * radius, QVector2D(0, 0)));

    current_area = shape.GetArea();

    this->style = style;
    this->mass = mass;
    this->gas_const = gas_const;
    this->shell_bounce = shell_bounce;
    this->shell_rigidity = shell_rigidity;
}

IGBall::IGBall() {}

QRect IGBall::GetBoundingRect() const
{
    return shape.GetBoundingRect().toRect();
}

void IGBall::WidenInspectorContext()
{
    Inspector::AddHeader("ideal gas ball", LARGE_HEADER);
    Inspector::AddParam("m/d", mass, 1e3d, 1e6d);
    Inspector::AddParam("GC", gas_const, 1.0, 1000.0);
    Inspector::AddParam("SB", shell_bounce, 0.0, 1.0);
    Inspector::AddParam("SR", shell_rigidity, 1.0, 1000.0);
    Inspector::AddLabel("area", get_label_string(current_area), &area_label_manager);
    Inspector::AddLabel("density", get_label_string(current_density), &density_label_manager);
    style.WidenInspectorContext();
}

bool IGBall::ContainsPoint(const QPoint &point) const
{
    return shape.ContainsPoint(QVector2D(point));
}

PhysicalBody *IGBall::Clone() const
{
    IGBall *clone = new IGBall();
    clone->style = style;
    clone->shape = shape;
    clone->mass = mass;
    clone->gas_const = gas_const;
    clone->shell_bounce = shell_bounce;
    clone->shell_rigidity = shell_rigidity;
    clone->current_area = -1;
    clone->current_density = -1;
    return clone;
}

void IGBall::SolveCollision(PhysicalBody *another)
{

}

void IGBall::KeepSceneBorders(const QRect &world_rect)
{
    for(auto &point : shape.GetPoints())
        point.KeepBorders(world_rect, shell_bounce);
}

void IGBall::ApplyInternalRestrictions(double delta_time)
{
    shape.UpdatePositions(delta_time);
}

void IGBall::ApplyGravity(double air_density, double g, double delta_time)
{
    current_area = shape.GetArea();
    area_label_manager.ChangeText(get_label_string(current_area));
    current_density = mass / current_area;
    density_label_manager.ChangeText(get_label_string(current_density));

    shape.AddVelocity(QVector2D(0, (1 - air_density / current_density) * g * delta_time));
}

void IGBall::MoveBy(const QPoint &offset)
{
    shape.MoveBy(QVector2D(offset));
}

void IGBall::AddMomentum(const QPoint &momentum)
{

}

void IGBall::Draw(QPainter &painter)
{
    shape.Draw(painter, style);
}

QPoint IGBall::GetLocalCoordinate(const QPoint &global_coordinate) const
{
    return global_coordinate - shape.GetConstPoints()[0].position.toPoint();
}

QPoint IGBall::GetGlobalCoordinate(const QPoint &local_coordinate) const
{
    return local_coordinate + shape.GetConstPoints()[0].position.toPoint();
}

