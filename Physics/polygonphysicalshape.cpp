#include "polygonphysicalshape.h"
#include <iostream>

PolygonPhysicalShape::PolygonPhysicalShape() : points() {}

vector<PolyPoint> &PolygonPhysicalShape::GetPoints()
{
    return points;
}

const vector<PolyPoint> &PolygonPhysicalShape::GetConstPoints() const
{
    return points;
}

QRectF PolygonPhysicalShape::GetBoundingRect() const
{
    if(points.size() == 0)
        return QRectF();

    float left = points[0].position.x();
    float right = points[0].position.x();
    float top = points[0].position.y();
    float bottom = points[0].position.y();

    for(auto &point : points)
    {
        left = min(left, point.position.x());
        right = max(right, point.position.x());
        top = min(top, point.position.y());
        bottom = max(bottom, point.position.y());
    }

    return QRectF(left, top, right - left, bottom - top);
}

static bool CheckUppperRaycast(QVector2D origin, QVector2D p1, QVector2D p2)
{
    if((origin.x() <= min(p1.x(), p2.x())) || (origin.x() >= max(p1.x(), p2.x())))
        return false;

    QVector2D a = p2 - p1;
    double k = a.y() / a.x();
    double proj_y = p1.y() + k * (origin.x() - p1.x());

    return proj_y < origin.y();
}

bool PolygonPhysicalShape::ContainsPoint(const QVector2D &point) const
{
    if(points.size() < 3)
        return false;

    if(!GetBoundingRect().contains(point.x(), point.y()))
        return false;

    bool result = false;

    result ^= CheckUppperRaycast(point, points[0].position, points[points.size() - 1].position);
    for(int i = 1; i < points.size(); i++)
        result ^= CheckUppperRaycast(point, points[i - 1].position, points[i].position);

    return result;
}

static inline float XProduct(QVector2D left, QVector2D right)
{
    return left.x() * right.y() - left.y() * right.x();
}

static bool GetTwoLinesIntersectionPoint(QVector2D l1s, QVector2D l1e, QVector2D l2s, QVector2D l2e, QVector2D &intersection_point)
{
    QVector2D b = l2s - l1s;
    QVector2D a1 = l1e - l1s;
    QVector2D a2 = l2e - l2s;

    float x = XProduct(a1, a2);
    if(x == 0)
        return false;

    float t1 = XProduct(b, a2) / x;
    float t2 = XProduct(b, a1) / x;

    if((t1 > 0) && (t1 < 1) && (t2 > 0) && (t2 < 1))
    {
        intersection_point = l1s + t1 * a1;
        return true;
    }
    return false;
}

void PolygonPhysicalShape::GetSideBySideIntersectionPoints(const PolygonPhysicalShape &another, vector<QVector2D> &points) const
{
    QVector2D intersection_point;
    for(int i = 0; i < this->points.size(); i++)
    {
        for(int j = 0; j < another.points.size(); j++)
        {
            if(GetTwoLinesIntersectionPoint(
                this->points[i == 0 ? this->points.size() - 1 : i - 1].position,
                this->points[i].position,
                another.points[j == 0 ? another.points.size() - 1 : j - 1].position,
                another.points[j].position,
                intersection_point
                ))
            {
                points.push_back(intersection_point);
            }
        }
    }
}

void PolygonPhysicalShape::Draw(QPainter &painter, const DrawingStyle &style) const
{
    painter.setBrush(Qt::SolidPattern);
    painter.setBrush(style.main_color);
    QRectF bounding_rect = GetBoundingRect();
    double border_thickness = min(style.border_thickness, min(bounding_rect.width(), bounding_rect.height()));
    painter.setPen(QPen(style.border_color, border_thickness));

    while(drawing_points.size() < points.size())
        drawing_points.push_back(QPoint());

    for(int i = 0; i < points.size(); i++)
        drawing_points[i] = points[i].position.toPoint();
    painter.drawPolygon(drawing_points.data(), points.size());
    painter.setPen(QPen(Qt::red, 3));
    for (int i = 0; i < debuglines.size(); i++) {
        painter.drawLine(debuglines[i]);
    }
}

void PolygonPhysicalShape::UpdatePositions(double delta_time)
{
    for(auto &point : points)
        point.position += point.velocity * delta_time;
}

void PolygonPhysicalShape::MoveBy(const QVector2D &offset)
{
    for(auto &point : points)
        point.position += offset;
}

void PolygonPhysicalShape::AddVelocity(const QVector2D &delta_velocity)
{
    for(auto &point : points)
        point.velocity += delta_velocity;
}

double crossProduct(QVector2D a, QVector2D b) {
    return (a.x() * b.y()) - (a.y() * b.x());
}

double PolygonPhysicalShape::getIntersectionArea(PolygonPhysicalShape& other) {
    int start_point = 0;
    for (; start_point < this->points.size(); start_point++) {
        if (other.ContainsPoint(this->points[start_point].position)) {
            break;
        }
    }
    if (start_point == this->points.size()) {
        return this->ContainsPoint(other.points[0].position) ? other.GetArea() : 0;
    }

    int in_l = start_point-1, in_h = start_point+1;

    if (in_l < 0) {
        in_l = this->points.size() - 1;
        for (; in_l >= 0; in_l--) {
            if (!other.ContainsPoint(this->points[in_l].position)) {
                break;
            }
        }
        if (in_l == 0) {
            return this->GetArea();
        }
    }

    while (in_h < this->points.size() && other.ContainsPoint(this->points[in_h].position)) {
        in_h++;
    }
    QVector2D in_h_point = this->points[in_h-1].position;
    QVector2D out_h_point = this->points[in_h % this->points.size()].position;
    in_h--;

    QVector2D out_l_point = this->points[in_l].position;
    in_l = (in_l+1) % this->points.size();
    QVector2D in_l_point = this->points[in_l].position;

    QVector2D xpoint_l;
    QVector2D xpoint_h;

    int other_inh = -1, other_inl = -1;

    for(int i = 1; i < other.points.size(); i++) {
        if (GetTwoLinesIntersectionPoint(in_l_point, out_l_point, other.points[i-1].position, other.points[i].position, xpoint_l)) {
            other_inl = i;
        }
        if (GetTwoLinesIntersectionPoint(in_h_point, out_h_point, other.points[i-1].position, other.points[i].position, xpoint_h)) {
            other_inh = i;
        }

    }
    if (GetTwoLinesIntersectionPoint(in_l_point, out_l_point, other.points[0].position, other.points[other.points.size()-1].position, xpoint_l)) {
        other_inl = 0;
    }
    if (GetTwoLinesIntersectionPoint(in_h_point, out_h_point, other.points[0].position, other.points[other.points.size()-1].position, xpoint_h)) {
        other_inh = 0;
    }

    if (other_inl == -1 || other_inh == -1) {
        return 0;
    }

    //this->debuglines.clear();
    //this->debuglines.push_back(QLine(this->points[in_l].position.toPoint(), this->points[in_h].position.toPoint()));
    //this->debuglines.push_back(QLine(in_l_point.toPoint(), out_l_point.toPoint()));

    //this->debuglines.push_back(QLine(xpoint_l.toPoint(), xpoint_h.toPoint()));

    double r = 0;
    for (int i  = in_l; i != in_h;) {
        int new_i = (i+1) % this->points.size();
        if (!other.ContainsPoint(this->points[i].position)) {
            std::cout << "BUG DETECTED" << std::endl;
            return 0;
        }
        r += crossProduct(this->points[i].position - xpoint_l, this->points[new_i].position - xpoint_l);
        i = new_i;
    }
    r += crossProduct(this->points[in_h].position - xpoint_l, xpoint_h - xpoint_l);

    if (abs(r) > 100000) {
        debuglines.clear();
        //for (int i  = in_l; i < in_h; i++) {
        //   debuglines.push_back(QLine(this->points[i].position.toPoint(), xpoint_l.toPoint()/*this->points[i+1].position.toPoint())*/));
        //}
        debuglines.push_back(QLine(in_l_point.toPoint(), out_l_point.toPoint()));
        debuglines.push_back(QLine(other.points[other_inl].position.toPoint(), other.points[other_inl-1].position.toPoint()));
        debuglines.push_back(QLine(xpoint_l.toPoint(), xpoint_h.toPoint()));
    }

    if (other_inl == other_inh) {
        return abs(r);
    }

    if (this->ContainsPoint(other.points[other_inl].position)) {
        //cout << "<" << r << " " << (other_inh - other_inl + other.points.size()) % other.points.size() << " " << other.points.size() << " ";;
        other_inh = other_inh - 1;
        if (other_inh < 0) {
            other_inh = other.points.size() - 1;
        }

        for (int i = other_inl; i != other_inh;) {
            int new_i = (i + 1) % other.points.size();
            if (!this->ContainsPoint(other.points[new_i].position)) {
                std::cout << "BUG DETECTED" << std::endl;
                return abs(r);
            }
            //this->debuglines.push_back(QLine(other.points[i].position.toPoint(), other.points[new_i].position.toPoint()));
            r -= crossProduct(other.points[i].position - xpoint_l, other.points[new_i].position - xpoint_l);
            i = new_i;
        }
        r -= crossProduct(other.points[other_inh].position - xpoint_l, xpoint_h - xpoint_l);
    }
    else {
        //cout << ">" << r << " " << (other_inl - other_inh + other.points.size()) % other.points.size() << " " << other.points.size() << " ";
        other_inl = other_inl - 1;
        if (other_inl < 0) {
            other_inl = other.points.size() - 1;
        }
        if (!this->ContainsPoint(other.points[other_inl].position)) {
            std::cout << "BUG DETECTED" << std::endl;
            return abs(r);
        }
        for (int i = other_inl; i != other_inh;) {
            int new_i = (i == 0) ? other.points.size()-1 : (i - 1);
            if (!this->ContainsPoint(other.points[new_i].position)) {
                std::cout << "BUG DETECTED" << std::endl;
                return abs(r);
            }

            //this->debuglines.push_back(QLine(other.points[i].position.toPoint(), other.points[new_i].position.toPoint()));
            r -= crossProduct(other.points[i].position - xpoint_l, other.points[new_i].position - xpoint_l);
            i = new_i;
        }
        r -= crossProduct(other.points[other_inh].position - xpoint_l, xpoint_h - xpoint_l);

    }
    cout << abs(r) << endl;

    return abs(r);
}


double PolygonPhysicalShape::GetArea() const
{
    if(points.size() < 3)
        return 0;

    float S = points[points.size() - 1].position.x() * points[0].position.y()
              - points[0].position.x() * points[points.size() - 1].position.y();

    for(int i = 1; i < this->points.size(); i++)
        S += points[i - 1].position.x() * points[i].position.y()
            - points[i].position.x() * points[i - 1].position.y();

    return abs(S) / 2;
}

QVector2D PolygonPhysicalShape::GetCenter() const
{
    QVector2D center(0, 0);

    for(auto &point : points)
        center += point.position;

    return center / points.size();
}

void PolygonPhysicalShape::GetSideBySideIntersectionInfo(PolygonPhysicalShape &another, vector<LinesIntersectionInfo> &info)
{
    QVector2D intersection_point;
    for(int i = 0; i < points.size(); i++)
    {
        for(int j = 0; j < another.points.size(); j++)
        {
            PolyPoint &fls = points[i == 0 ? points.size() - 1 : i - 1];
            PolyPoint &fle = points[i];
            PolyPoint &sls = another.points[j == 0 ? another.points.size() - 1 : j - 1];
            PolyPoint &sle = another.points[j];
            if(GetTwoLinesIntersectionPoint(fls.position, fle.position, sls.position, sle.position, intersection_point))
            {
                info.push_back(LinesIntersectionInfo(fls, fle, sls, sle, intersection_point));
            }
        }
    }
}

void PolygonPhysicalShape::GetSelfIntersectionInfo(vector<LinesIntersectionInfo> &info)
{
    QVector2D intersection_point;
    for(int i = 0; i < points.size(); i++)
    {
        //skip adjacent sides
        for(int j = i + 2; j < points.size(); j++)
        {
            PolyPoint &fls = points[i == 0 ? points.size() - 1 : i - 1];
            PolyPoint &fle = points[i];
            PolyPoint &sls = points[j == 0 ? points.size() - 1 : j - 1];
            PolyPoint &sle = points[j];
            if(GetTwoLinesIntersectionPoint(fls.position, fle.position, sls.position, sle.position, intersection_point))
            {
                info.push_back(LinesIntersectionInfo(fls, fle, sls, sle, intersection_point));
            }
        }
    }
}

void PolygonPhysicalShape::LimitVelocity(double limit)
{
    double limit2 = limit * limit;
    for(auto &point : points)
        if(point.velocity.lengthSquared() > limit2)
            point.velocity *= limit / point.velocity.length();
}

QVector2D PolygonPhysicalShape::GetCenterVelocity() const
{
    if(points.size() == 0)
        return QVector2D(0, 0);

    QVector2D result(0, 0);
    for(auto &point : points)
        result += point.velocity;

    return result / points.size();
}
