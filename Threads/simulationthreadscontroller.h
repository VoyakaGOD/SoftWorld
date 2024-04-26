#ifndef SIMULATIONTHREADSCONTROLLER_H
#define SIMULATIONTHREADSCONTROLLER_H

#include <QMutex>
#include <QWidget>
#include <QtGlobal>
#include "physicalthread.h"
#include "drawingthread.h"
#include "Inspector/inspector.h"

#define CONNECT QObject::connect

//udelta in microseconds
class SimulationThreadsController
{
private:
    static QMutex data_synchronizer;
    static QMutex threads_synchronizer;
    static SoftScene *scene;
    static unsigned long p_udelta;
    static PhysicalThread *p_thread;
    static QWidget *scene_view;
    static unsigned long d_udelta;
    static DrawingThread *d_thread;
    static int fps_limit;               //only for Inspector

public:
    static void WidenInspectorContext();
    static EditingManager *GetEditingManager();
    static void Mount(SoftScene *scene, unsigned long physics_udelta, QWidget *scene_view, unsigned long drawing_udelta);
    static SoftScene *GetScene();
    static QWidget *GetSceneView();
    static unsigned long GetPhysicsDelta();
    static void SetPhysicsDelta(unsigned long udelta);
    static unsigned long GetDrawingDelta();
    static void SetDrawingDelta(unsigned long udelta);
    static void Run();
    static void Stop();

private:
    SimulationThreadsController();

    friend class SimulationThreadsControllerEditingManager;
};

#endif // SIMULATIONTHREADSCONTROLLER_H
