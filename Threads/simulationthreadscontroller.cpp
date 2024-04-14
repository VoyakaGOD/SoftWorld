#include "simulationthreadscontroller.h"

QMutex SimulationThreadsController::synchronizer;
SoftScene *SimulationThreadsController::scene;
unsigned long SimulationThreadsController::p_udelta;
PhysicalThread *SimulationThreadsController::p_thread;

SimulationThreadsController::SimulationThreadsController() {}

void SimulationThreadsController::Mount(SoftScene *scene, unsigned long physics_udelta)
{
    QMutexLocker lock(&synchronizer);

    SimulationThreadsController::scene = scene;
    p_udelta = physics_udelta;
    p_thread = nullptr;
}

SoftScene *SimulationThreadsController::GetScene()
{
    return scene;
}

unsigned long SimulationThreadsController::GetPhysicsDelta()
{
    QMutexLocker lock(&synchronizer);

    return p_udelta;
}

void SimulationThreadsController::SetPhysicsDelta(unsigned long udelta)
{
    QMutexLocker lock(&synchronizer);

    p_udelta = udelta;
}

void SimulationThreadsController::Run()
{
    QMutexLocker lock(&synchronizer);

    if(p_thread)
        return;

    p_thread = new PhysicalThread();
    p_thread->start();
}

void SimulationThreadsController::Stop()
{
    QMutexLocker lock(&synchronizer);

    if(p_thread)
        delete p_thread;
    p_thread = nullptr;
}
