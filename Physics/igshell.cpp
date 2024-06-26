#include <Serialize/ser_class_enums.h>

#include "igshell.h"

struct IGShellData {
    obj_fixed_data_len_t size;
    DrawingStyle style;
    double mass;
    double gas_const;
    double shell_bounce;
    double shell_rigidity;
    double initial_part_length;
    double reconstruction_radius;
    int reconstruction_detailing;
};

static IGShellData default_data {
    .size = 0,
    .style = DrawingStyle(),
    .mass = 1000,
    .gas_const = 200,
    .shell_bounce = 0.5,
    .shell_rigidity = 1,
    .initial_part_length = 60,
    .reconstruction_radius = 10,
    .reconstruction_detailing = 50
};

static QString get_label_string(double value)
{
    return (value > 0) ? QString::number(value) : "...";
}

IGShell::IGShell(QVector2D position, double radius, int detailing, DrawingStyle style,
    double mass, double gas_const, double shell_bounce, double shell_rigidity)
{
    Reconstruct(position, radius, detailing);

    this->style = style;
    this->mass = mass;
    this->gas_const = gas_const;
    this->shell_bounce = shell_bounce;
    this->shell_rigidity = shell_rigidity;
}

IGShell::IGShell() {}

void IGShell::Reconstruct(QVector2D position, double radius, int detailing)
{
    points.clear();
    double dphi = 2 * M_PI / detailing;
    for(int i = 0; i < detailing; i++)
        points.push_back(PolyPoint(position + QVector2D(cos(dphi * i), sin(dphi * i)) * radius, QVector2D(0, 0)));

    current_area = GetArea();
    current_density = -1;
    initial_part_length = radius * sqrt(2 - 2*cos(dphi));
}

size_t IGShell::GetSavedSize() const {
    return PhysicalBody::GetSavedSize() + sizeof(saved_obj_id_t)
           + sizeof(IGShellData) + PolygonPhysicalShape::GetSavedSize() + sizeof(SAVED_OBJ_NONE);
}

void IGShell::SaveData(DataStorageWriter &writer) const {
    PhysicalBody::SaveData(writer);
    IGShellData* data = (IGShellData*)(writer.data);
    WRITER_MOVE_BYTES(writer, sizeof(*data))
    *data = (IGShellData){.size = sizeof(*data), .style = this->style
                        , .mass = this->mass, .gas_const = this->gas_const
                        , .shell_bounce = this->shell_bounce, .shell_rigidity = this->shell_rigidity
                        , .initial_part_length = this->initial_part_length
                        , .reconstruction_radius = this->reconstruction_radius, .reconstruction_detailing = this->reconstruction_detailing
                        };
    PolygonPhysicalShape::SaveData(writer);
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_NONE);
}

IGShell::IGShell(DataStorageReader &reader) :
PhysicalBody(reader), style(GETDATA(reader, IGShellData, style)),
mass(GETDATA(reader, IGShellData, mass)), gas_const(GETDATA(reader, IGShellData, gas_const)),
shell_bounce(GETDATA(reader, IGShellData, shell_bounce)), shell_rigidity(GETDATA(reader, IGShellData, shell_rigidity)),
initial_part_length(GETDATA(reader, IGShellData, initial_part_length)),
reconstruction_radius(GETDATA(reader, IGShellData, reconstruction_radius)), reconstruction_detailing(GETDATA(reader, IGShellData, reconstruction_detailing)),
current_area(-1), current_density(-1)
{

    READER_MOVE_BYTES(reader, ((IGShellData*)reader.data)->size, {last_deserialize_error = DESERR_BOUNDS; return;})
    PolygonPhysicalShape::Deserialize(reader);
}


QRectF IGShell::GetBoundingRect() const
{
    return PolygonPhysicalShape::GetBoundingRect();
}

void IGShell::WidenInspectorContext()
{
    Inspector::AddHeader("shell with ideal gas", LARGE_HEADER);
    Inspector::AddParam("m/d", mass, 1e2d, 1e7d);
    Inspector::AddParam("GC", gas_const, 1e4d, 1e5d);
    Inspector::AddParam("SB", shell_bounce, 0.0, 3.0);
    Inspector::AddParam("SR", shell_rigidity, 250.0, 20000.0);
    style.WidenInspectorContext();
    Inspector::AddHeader("volatile parameters", NORMAL_HEADER);
    Inspector::AddLabel("area", get_label_string(current_area), &area_label_manager);
    Inspector::AddLabel("density", get_label_string(current_density), &density_label_manager);
    Inspector::AddHeader("reconstruction", NORMAL_HEADER);
    Inspector::AddParam("new radius", reconstruction_radius, 40.0, 200.0);
    Inspector::AddParam("new detailing", reconstruction_detailing, 11, 100);
    Inspector::AddAction("apply", [this](){ Reconstruct(GetCenter(), reconstruction_radius, reconstruction_detailing); });
}

bool IGShell::ContainsPoint(const QPoint &point) const
{
    return PolygonPhysicalShape::ContainsPoint(QVector2D(point));
}

PhysicalBody *IGShell::Clone() const
{
    IGShell *clone = new IGShell();
    clone->style = style;
    clone->points = points;
    clone->mass = mass;
    clone->gas_const = gas_const;
    clone->shell_bounce = shell_bounce;
    clone->shell_rigidity = shell_rigidity;
    clone->current_area = -1;
    clone->current_density = -1;
    clone->initial_part_length = initial_part_length;
    return clone;
}

void IGShell::SolveCollision(PhysicalBody *another)
{
    PolygonPhysicalShape *poly = dynamic_cast<PolygonPhysicalShape *>(another);
    if(poly == NULL)
        return;

    vector<LinesIntersectionInfo> intersections;
    GetSideBySideIntersectionInfo(*poly, intersections);
    for(auto intersection : intersections)
    {
        QVector2D avg = (intersection.first_line_start.velocity + intersection.first_line_end.velocity +
                         intersection.second_line_start.velocity + intersection.second_line_end.velocity) / 4;
        intersection.first_line_start.velocity = avg;
        intersection.first_line_end.velocity = avg;
        intersection.second_line_start.velocity = avg;
        intersection.second_line_end.velocity = avg;

        QVector2D another_tangent = intersection.second_line_end.position - intersection.second_line_start.position;
        QVector2D another_normal(-another_tangent.y(), another_tangent.x());
        another_normal.normalize();

        //bad solution, but some times we should use simple approaches
        intersection.first_line_start.position -= another_normal;
        intersection.first_line_end.position -= another_normal;
    }
}

void IGShell::KeepSceneBorders(const QRect &world_rect)
{
    for(auto &point : points)
        point.KeepBorders(world_rect, shell_bounce);
}

void IGShell::ApplyInternalRestrictions(double delta_time)
{
    LimitVelocity(500);
    UpdatePositions(delta_time);

    vector<LinesIntersectionInfo> self_intersections;
    GetSelfIntersectionInfo(self_intersections);
    for(auto self_intersection : self_intersections)
    {
        QVector2D avg = (self_intersection.first_line_start.velocity + self_intersection.first_line_end.velocity +
                         self_intersection.second_line_start.velocity + self_intersection.second_line_end.velocity) / 4;
        self_intersection.first_line_start.velocity = avg;
        self_intersection.first_line_end.velocity = avg;
        self_intersection.second_line_start.velocity = avg;
        self_intersection.second_line_end.velocity = avg;
    }

    for(int i = 0; i < points.size(); i++)
    {
        PolyPoint &current = points[i];
        PolyPoint &next = points[(i + 1) % points.size()];
        QVector2D tangent = next.position - current.position;
        double a_n = tangent.length() * pow(gas_const / current_area, 3);
        double a_t = shell_rigidity * (tangent.length() / initial_part_length - 1);
        tangent.normalize();
        QVector2D normal(tangent.y(), -tangent.x());
        current.velocity += (normal * a_n + tangent * a_t) * delta_time;
        next.velocity += (normal * a_n - tangent * a_t) * delta_time;
    }
}

void IGShell::ApplyGravity(double air_density, double g, double delta_time)
{
    current_area = GetArea() + 1;//to avoid division by zero
    area_label_manager.ChangeText(get_label_string(current_area));
    current_density = mass / current_area;
    density_label_manager.ChangeText(get_label_string(current_density));

    AddVelocity(QVector2D(0, (1 - air_density / current_density) * g * delta_time));
}

void IGShell::MoveBy(const QPoint &offset)
{
    PolygonPhysicalShape::MoveBy(QVector2D(offset));
}

void IGShell::AddMomentum(const QVector2D &momentum)
{
    AddVelocity(QVector2D(momentum));
}

void IGShell::Draw(QPainter &painter)
{
    PolygonPhysicalShape::Draw(painter, style);
}

QPoint IGShell::GetLocalCoordinate(const QPoint &global_coordinate) const
{
    return global_coordinate - GetCenter().toPoint();
}

QPoint IGShell::GetGlobalCoordinate(const QPoint &local_coordinate) const
{
    return local_coordinate + GetCenter().toPoint();
}

QVector2D IGShell::GetCenterVelocity() const
{
    return PolygonPhysicalShape::GetCenterVelocity();
}
