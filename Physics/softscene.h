#ifndef SOFTSCENE_H
#define SOFTSCENE_H

#include <mutex>
#include "physicalbody.h"
#include "Inspector/inspector.h"
#include "Inspector/editingmanager.h"

class SoftScene
{
private:
    mutable mutex synchronizer;
    QRect world_rect;
    list<PhysicalBody*> bodies;
    double air_density;
    double g;

public:
    SoftScene(const QRect &world_rect, double air_density, double g);
    ~SoftScene();
    void Draw(QPainter &painter) const;
    void DoNextStep(double delta_time);
    void AddBody(PhysicalBody *body);
    void RemoveBody(PhysicalBody *body);
    void WidenInspectorContext();
    PhysicalBody *GetBodyAt(const QPoint &point) const;
    void Lock();
    void Unlock();
};

#endif // SOFTSCENE_H
