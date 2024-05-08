
#include "springmassbody.h"
#include <iostream>
#include <Serialize/ser_class_enums.h>
#include <Serialize/serialize.h>
#include <Serialize/serialize_special.h>

struct SpringMassBodyData {
    obj_fixed_data_len_t size;
    DrawingStyle drawing_style;
    double xscale, yscale, stiffness, resistance, density;
};

static SpringMassBodyData default_data {
    .size = 0, .drawing_style = DrawingStyle(),
    .xscale = 10, .yscale = 10, .stiffness = 500, .resistance = 10,
    .density = 1
};

struct SavedSpringBodyRow {
    int y; int startx; int size;
    PolyPoint points[];
};

const double collision_hardness_factor = 100;

static double dt = 0; // TODO: BIG COSTYL

SpringMassBody::SpringMassBody() {
    int xsize = 10, ysize = 10;

    this->rows = vector<SpringBodyRow> (ysize);
    for (int i = 0; i < this->rows.size(); i++) {
        this->rows[i].y = i;
        this->rows[i].startx = 0;
        this->rows[i].atoms = vector<PolyPoint>(xsize);
        for (int j = 0; j < xsize; j++) {
            this->rows[i].atoms[j].position = QVector2D(this->xscale * (this->rows[i].startx + j), this->yscale * (this->rows[i].y));
        }
    }
    this->points = vector<PolyPoint>(this->rows[0].size() + this->rows[this->rows.size()-1].size() + ((this->rows.size() - 1) * 2));
    this->RecalculatePolyPoints();
}

size_t SpringMassBody::GetSavedSize() const {
    size_t res = PhysicalBody::GetSavedSize() + sizeof(SpringMassBodyData) + sizeof(saved_obj_id_t);
    res += sizeof(SAVED_OBJ_PKG) + lenAsShortLength(this->rows.size());
    for (int i = 0; i < this->rows.size(); i++) {
        size_t row_size = sizeof(SavedSpringBodyRow) + (sizeof(PolyPoint) * this->rows[i].size());
        res += sizeof(SAVED_OBJ_STR) + lenAsShortLength(row_size) + row_size;
    }
    res += sizeof(SAVED_OBJ_NONE);
    return res;
}

SpringMassBody::SpringMassBody(DataStorageReader &reader):
    PhysicalBody(reader),
    xscale(GETDATA(reader, SpringMassBodyData, xscale)), yscale(GETDATA(reader, SpringMassBodyData, yscale)),
    stiffness(GETDATA(reader, SpringMassBodyData, stiffness)), resistance(GETDATA(reader, SpringMassBodyData, resistance)),
    density(GETDATA(reader, SpringMassBodyData, density)), style(GETDATA(reader, SpringMassBodyData, drawing_style))
    {
    this->diagscale = sqrt(this->xscale*this->xscale + this->yscale * this->yscale);

    READER_MOVE_BYTES(reader, ((SpringMassBodyData*)reader.data)->size, {last_deserialize_error = DESERR_BOUNDS; return;});
    size_t row_cnt = 0;
    int res;
    if ((res = readPkgHeader(reader, &row_cnt))) {
        last_deserialize_error = (DeserializeError)res;
        return;
    }
    for (int i = 0; i < row_cnt; i++) {
        size_t row_size_bytes = 0;
        saved_obj_id_t id;
        READER_READVAL(reader, saved_obj_id_t, id, {last_deserialize_error = DESERR_BOUNDS; return; })
        if (id != SAVED_OBJ_STR) {
            last_deserialize_error = DESERR_INVTYPE;
            return;
        }
        row_size_bytes = readShortLength(reader);
        SavedSpringBodyRow* s_row = (SavedSpringBodyRow*)reader.data;

        READER_MOVE_BYTES(reader, row_size_bytes, {last_deserialize_error = DESERR_BOUNDS; return; })

        if (sizeof(SavedSpringBodyRow) + (sizeof(PolyPoint) * s_row->size) != row_size_bytes) {
            last_deserialize_error = DESERR_BADDATA;
            return;
        }
        this->rows.push_back(SpringBodyRow());
        this->rows[i].startx = s_row->startx;
        this->rows[i].y = s_row->y;
        for (int j = 0; j < s_row->size; j++) {
            this->rows[i].atoms.push_back(s_row->points[j]);
        }
    }
    this->points = vector<PolyPoint>(this->rows[0].size() + this->rows[this->rows.size()-1].size() + ((this->rows.size() - 1) * 2));
    this->RecalculatePolyPoints();
}

void SpringMassBody::SaveData(DataStorageWriter &writer) const {
    PhysicalBody::SaveData(writer);
    SpringMassBodyData* data = (SpringMassBodyData*)(writer.data);
    WRITER_MOVE_BYTES(writer, sizeof(*data));

    *data = (SpringMassBodyData){
        .size = sizeof(*data),
        .drawing_style = this->style,
        .xscale = this->xscale, .yscale = this->yscale,
        .stiffness = this->stiffness, .resistance = this->resistance,
        .density = this->density,
    };

    savePkgHeader(this->rows.size(), writer);
    for (int i = 0; i < this->rows.size(); i++) {
        WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_STR);
        size_t len = sizeof(SavedSpringBodyRow)
             + (this->rows[i].size() * sizeof(PolyPoint));

        writeShortLength(writer,len);
        SavedSpringBodyRow* saved_row = (SavedSpringBodyRow*)writer.data;
        saved_row->y = this->rows[i].y;
        saved_row->startx = this->rows[i].startx;
        saved_row->size = this->rows[i].size();

        for (int j = 0; j < this->rows[i].size(); j++) {
            saved_row->points[j] = this->rows[i].atoms[j];
        }
        WRITER_MOVE_BYTES(writer, len);
    }
    WRITER_APPEND(writer, saved_obj_id_t, SAVED_OBJ_NONE);
}

void SpringMassBody::RecalculatePolyPoints() {
    int poly_n = 0;
    for (int i = 0; i < this->rows[0].size(); i++) {
        this->points[poly_n++] = this->rows[0][i];
    }
    for (int i = 0; i < this->rows.size(); i++) {
        this->points[poly_n++] = this->rows[i][this->rows[i].size()-1];
    }
    for (int i = this->rows[this->rows.size()-2].size()-1; i > 0; i--) {
        this->points[poly_n++] = this->rows[this->rows.size()-1][i];
    }
    for (int i = this->rows.size()-1; i > 0; i--) {
        this->points[poly_n++] = this->rows[i][0];
    }
}

static void applyforce(PolyPoint &p1, PolyPoint &p2, double scale, double stiffness, double resistance, double mass) {
    QVector2D line = (p1.position - p2.position);
    QVector2D linedir = line.normalized();
    double delta_v = QVector2D::dotProduct(p1.velocity - p2.velocity, linedir);
    double resforce = (delta_v * resistance);
    if (abs(resforce) > abs(mass * delta_v / 2)) {
        resforce = mass * delta_v / 2;
    }
    double force = ((line.length() - scale) * stiffness) + resforce;
    p1.velocity -= (linedir * force / mass);
    p2.velocity += (linedir * force / mass);
}

void SpringMassBody::ApplyInternalRestrictions(double delta_time) {
    dt = delta_time;
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            this->rows[i][j].position += this->rows[i][j].velocity * delta_time;
        }
    }
    this->RecalculatePolyPoints();

    for (int j = 1; j < this->rows[0].size(); j++) {
        applyforce(this->rows[0][j-1], this->rows[0][j], this->xscale, this->stiffness, this->resistance, this->density / delta_time);
    }

    for (int i = 1; i < this->rows.size(); i++) {
        int prev_start_j = this->rows[i-1].startx - this->rows[i].startx;
        int start_j = max(0, prev_start_j);
        int end_j = min(this->rows[i].size(), this->rows[i-1].size() + prev_start_j);

        applyforce(this->rows[i-1][start_j - prev_start_j], this->rows[i][start_j], this->yscale, this->stiffness, this->resistance, this->density / delta_time);
        if (prev_start_j < 0) {
            applyforce(this->rows[i-1][start_j - prev_start_j - 1], this->rows[i][start_j], this->diagscale, this->stiffness, this->resistance, this->density / delta_time);
        }
        if (end_j - prev_start_j < this->rows[i-1].size()) {
            applyforce(this->rows[i-1][end_j - prev_start_j], this->rows[i][end_j-1], this->diagscale, this->stiffness, this->resistance, this->density / delta_time);
        }


        for (int j = start_j + 1; j < end_j; j++) {
            QVector2D v2 = this->rows[i][j-1].position - this->rows[i-1][prev_start_j + j-1].position;
            QVector2D v1 = this->rows[i-1][prev_start_j + j].position - this->rows[i-1][prev_start_j + j-1].position;

            if ((v1.x() * v2.y()) - (v2.x() * v1.y()) >= 0) {
                applyforce(this->rows[i][j-1], this->rows[i-1][prev_start_j + j], this->diagscale, this->stiffness, this->resistance, this->density / delta_time);
                applyforce(this->rows[i-1][prev_start_j + j-1], this->rows[i][j], this->diagscale, this->stiffness, this->resistance, this->density / delta_time);
            }
            else {
                applyforce(this->rows[i][j-1], this->rows[i-1][prev_start_j + j], this->diagscale, -this->stiffness / 10, this->resistance, this->density / delta_time);
                applyforce(this->rows[i-1][prev_start_j + j-1], this->rows[i][j], this->diagscale, -this->stiffness / 10, this->resistance, this->density / delta_time);
            }
            applyforce(this->rows[i][j-1], this->rows[i][j], this->xscale, this->stiffness, this->resistance, this->density / delta_time);
            applyforce(this->rows[i-1][prev_start_j + j], this->rows[i][j], this->yscale, this->stiffness, this->resistance, this->density / delta_time);
        }
    }
}

void SpringMassBody::KeepSceneBorders(const QRect &world_rect) {
    const double wall_hardness = 100;
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            if (this->rows[i][j].position.x() < world_rect.left()) {
                double vx = this->rows[i][j].velocity.x();
                //if (vx < 0)
                //    vx *= -1;
                vx += (world_rect.left() - this->rows[i][j].position.x()) * wall_hardness;
                this->rows[i][j].velocity.setX(vx);
            }
            if (this->rows[i][j].position.x() > world_rect.right()) {
                double vx = this->rows[i][j].velocity.x();
                //if (vx > 0)
                //   vx *= -1;
                vx += (world_rect.right() - this->rows[i][j].position.x()) * wall_hardness;
                this->rows[i][j].velocity.setX(vx);
            }
            if (this->rows[i][j].position.y() < world_rect.top()) {
                double vy = this->rows[i][j].velocity.y();
                //if (vy < 0)
                //   vy *= -1;
                vy += (world_rect.top() - this->rows[i][j].position.y()) * wall_hardness;
                this->rows[i][j].velocity.setY(vy);
            }
            if (this->rows[i][j].position.y() > world_rect.bottom()) {
                double vy = this->rows[i][j].velocity.y();
                //if (vy > 0)
                //   vy *= -1;
                vy += (world_rect.bottom() - this->rows[i][j].position.y()) * wall_hardness;
                this->rows[i][j].velocity.setY(vy);
            }
        }
    }
}

void SpringMassBody::ApplyGravity(double air_density, double g, double delta_time) {
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            this->rows[i][j].velocity.setY(this->rows[i][j].velocity.y() + (g * delta_time));
        }
    }
}

void SpringMassBody::Draw(QPainter &painter) {
    PolygonPhysicalShape::Draw(painter, this->style);

    painter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            painter.drawPoint(this->rows[i][j].position.toPoint());
        }
    }
}

void SpringMassBody::MoveBy(const QPoint &offset) {
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            this->rows[i][j].position += QVector2D(offset);
        }
    }
    this->RecalculatePolyPoints();
}

void SpringMassBody::AddMomentum(const QVector2D &momentum) {
    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
            this->rows[i][j].velocity += momentum;
        }
    }
}

static void expandrect(QPoint point, QRect &rect) {
    rect.setLeft(min(rect.left(), point.x()));
    rect.setRight(max(rect.right(), point.x()));
    rect.setTop(min(rect.top(), point.y()));
    rect.setBottom(max(rect.bottom(), point.y()));
}

QRectF SpringMassBody::GetBoundingRect() const {
  return PolygonPhysicalShape::GetBoundingRect();
}

bool SpringMassBody::ContainsPoint(const QPoint &point) const {
    return PolygonPhysicalShape::ContainsPoint(QVector2D(point));
}

PhysicalBody* SpringMassBody::Clone() const {
    return new SpringMassBody(*this);
}

void SpringMassBody::SolveCollision(PhysicalBody *another) {
    PolygonPhysicalShape* p_another = dynamic_cast<PolygonPhysicalShape*>(another);
    if (!p_another)
        return;

    vector<QVector2D> intpoints;
    PolygonPhysicalShape::GetSideBySideIntersectionPoints(*p_another, intpoints);
    if (intpoints.size() < 2) {
        return;
    }
    QVector2D collision_vector(intpoints[0].y() - intpoints[1].y(), intpoints[1].x() - intpoints[0].x());

    PolyPoint* colliding_point = nullptr;

    for (int i = 0; i < this->points.size(); i++) {
      if (p_another->ContainsPoint(this->points[i].position))
	    colliding_point = &(this->points[i]);
    }
    if (colliding_point == nullptr) {
        return;
    }

    if (QVector2D::dotProduct(collision_vector, colliding_point->position - intpoints[0]) < 0) {
      collision_vector *= -1;
    }
    collision_vector.normalize();

    double total_impulse = PolygonPhysicalShape::getIntersectionArea(*p_another)*collision_hardness_factor;
    //this->debuglines.push_back(QLine(intpoints[0].toPoint(), (intpoints[0] + collision_vector * 100).toPoint()));
    std::cout << "I" << total_impulse << std::endl;
    //return;


    double total_factor = 0;

    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
        if (p_another->ContainsPoint(this->rows[i][j].position)) {
            double factor = QVector2D::dotProduct(this->rows[i][j].position - intpoints[0], collision_vector);
            if (factor > 0)
                total_factor += factor;
            }
        }
    }

    total_impulse = total_impulse / total_factor;

    for (int i = 0; i < this->rows.size(); i++) {
        for (int j = 0; j < this->rows[i].size(); j++) {
	  if (p_another->ContainsPoint(this->rows[i][j].position)) {
            double factor = QVector2D::dotProduct(this->rows[i][j].position - intpoints[0], collision_vector);
            if (factor > 0)
                this->rows[i][j].velocity -= factor * collision_vector * total_impulse * dt / this->density;
            }
        }
    }

    return;
}

bool pointInTriangle(QVector2D a, QVector2D b, QVector2D c, QVector2D p) {
  double d1 = QVector2D::dotProduct(p-a, b-a);
  double d2 = QVector2D::dotProduct(p-b, c-b);
  double d3 = QVector2D::dotProduct(p-c, a-c);
  return (d1 >= 0 && d1 <= (b-a).lengthSquared()) &&
         (d2 >= 0 && d2 <= (c-b).lengthSquared()) &&
         (d3 >= 0 && d3 <= (a-c).lengthSquared());
}

QPoint translateCoordinatesTriangle(QVector2D o, QVector2D px, QVector2D py, const QVector2D coord, double xscale, double yscale) {
  return QPoint(xscale * QVector2D::dotProduct(px-o, coord - o) / (px-o).lengthSquared(), yscale * QVector2D::dotProduct(py-o, coord - o) / (py-o).lengthSquared());
}

QPoint SpringMassBody::GetLocalCoordinate(const QPoint &global_coordinate) const {
    for (int i = 1; i < this->rows.size(); i++) {
        int minx = max(this->rows[i].startx, this->rows[i-1].startx);
        int maxx = min(this->rows[i].startx + (int)this->rows[i].size(), this->rows[i-1].startx + (int)this->rows[i-1].size());
        for (int j = minx+1; j < maxx; j++) {
            QVector2D po1 = this->rows[i-1].atoms[j - this->rows[i-1].startx-1].position;
            QVector2D px = this->rows[i-1].atoms[j - this->rows[i-1].startx].position;
            QVector2D py = this->rows[i].atoms[j - this->rows[i].startx-1].position;
            QVector2D po2 = this->rows[i].atoms[j - this->rows[i].startx].position;
            if (pointInTriangle(po1, px, py, QVector2D(global_coordinate)) || pointInTriangle(po2, px, py, QVector2D(global_coordinate))) {
                QPoint r = translateCoordinatesTriangle(po1, px, py, QVector2D(global_coordinate), this->xscale, this->yscale) + QPoint((j - 1) * xscale, (i-1) * yscale);
                QPoint backpoint = this->GetGlobalCoordinate(r);
                return r;
            }
	    }
    }
    return QPoint(0, 0);
}
QPoint SpringMassBody::GetGlobalCoordinate(const QPoint &local_coordinate) const {
  int pointy = local_coordinate.y() / this->yscale, pointx = local_coordinate.x() / this->xscale;
  pointy = min(pointy, (int)this->rows.size() - 2);
  pointy = max(0, pointy);
  pointx = min(pointx, this->rows[pointy].startx + (int)this->rows[pointy].size() - 2);
  pointx = min(pointx, this->rows[pointy+1].startx + (int)this->rows[pointy+1].size() - 1);
  pointx = max(pointx, this->rows[pointy].startx);
  pointx = max(pointx, this->rows[pointy+1].startx);

  QVector2D rel_point = QVector2D(local_coordinate.x() - (this->xscale * pointx), local_coordinate.y() - (this->yscale * pointy));
  QVector2D xvector = this->rows[pointy].atoms[pointx + 1 - rows[pointy].startx].position -
    this->rows[pointy].atoms[pointx - rows[pointy].startx].position;
  QVector2D yvector = this->rows[pointy+1].atoms[pointx - rows[pointy+1].startx].position -
    this->rows[pointy].atoms[pointx - rows[pointy+1].startx].position;

  return (this->rows[pointy].atoms[pointx].position + (xvector * rel_point.x() / this->xscale) + (yvector * rel_point.y() / this->yscale)).toPoint();
};

void SpringMassBody::WidenInspectorContext() {
  Inspector::AddHeader("spring-mass body", LARGE_HEADER);
  Inspector::AddParam("Stiffness", this->stiffness, 0, 5000);
  Inspector::AddParam("Resistance", this->resistance, 0, 100);
  Inspector::AddParam("Density", this->density, 1, 100);
  this->style.WidenInspectorContext();
}

QVector2D SpringMassBody::GetCenterVelocity() const {
    QVector2D res(0, 0);
    int div = 0;
    for (int i = 0; i < this->rows.size(); i++) {
        div += this->rows[i].size();
        for (int j = 0; j < this->rows[i].size(); j++) {
            res += this->rows[i].atoms[j].velocity;
        }
    }
    return res / div;
}
