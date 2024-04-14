#ifndef SIMULATIONTHREADSCONTROLLER_H
#define SIMULATIONTHREADSCONTROLLER_H

#include <QMutex>
#include "physicalthread.h"
#include "Inspector/editingmanager.h"

//udelta in microseconds
class SimulationThreadsController
{
private:
    static QMutex synchronizer;
    static SoftScene *scene;
    static unsigned long p_udelta;
    static PhysicalThread *p_thread;

public:
    //static void WidenInspectorContext();
    static void Mount(SoftScene *scene, unsigned long physics_udelta);
    static SoftScene *GetScene();
    static unsigned long GetPhysicsDelta();
    static void SetPhysicsDelta(unsigned long udelta);
    //static void SetFPS(float fps);
    static void Run();
    static void Stop();

private:
    SimulationThreadsController();
};

#endif // SIMULATIONTHREADSCONTROLLER_H
