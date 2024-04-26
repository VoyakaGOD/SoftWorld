#ifndef SOFTSCENE_H
#define SOFTSCENE_H

#include <QMutex>
#include "physicalbody.h"
#include "Inspector/inspector.h"
#include "Inspector/editingmanager.h"

//time limit for try to lock
#define SOFT_SCENE_REQUEST_TIME_LIMIT 100 //milliseconds

class SoftScene
{
private:
    mutable QMutex synchronizer;
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
    void Lock() const;
    bool TryToLock() const;
    void Unlock() const;
};

#endif // SOFTSCENE_H
