#ifndef SOFTSCENE_H
#define SOFTSCENE_H

#include <QMutex>
#include "physicalbody.h"
#include "Inspector/inspector.h"
#include "Inspector/editingmanager.h"

//time limit for try to lock
#define SOFT_SCENE_REQUEST_TIME_LIMIT 100 //milliseconds

//You should call PrepareToDraw() before each Draw()
class SoftScene
{
private:
    mutable QMutex synchronizer;
    QRect world_rect;
    list<PhysicalBody*> bodies;
    double air_density;
    double g;
    mutable bool prepared_to_draw;

public:
    SoftScene(const QRect &world_rect, double air_density, double g);
    ~SoftScene();
    void PrepareToDraw(bool lock = true);
    void Draw(QPainter &painter) const;
    void DoNextStep(double delta_time);
    void AddBody(PhysicalBody *body);
    void RemoveBody(PhysicalBody *body);
    void WidenInspectorContext();
    PhysicalBody *GetBodyAt(const QPoint &point) const;
    void Lock();
    bool TryToLock();
    void Unlock();
};

#endif // SOFTSCENE_H
