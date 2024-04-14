#include "simulationthreadscontroller.h"

QMutex SimulationThreadsController::data_synchronizer;
QMutex SimulationThreadsController::threads_synchronizer;
SoftScene *SimulationThreadsController::scene;
unsigned long SimulationThreadsController::p_udelta;
PhysicalThread *SimulationThreadsController::p_thread;
QWidget *SimulationThreadsController::scene_view;
unsigned long SimulationThreadsController::d_udelta;
DrawingThread *SimulationThreadsController::d_thread;
int SimulationThreadsController::fps_limit;

class SimulationThreadsControllerEditingManager : public EditingManager
{
    void OnEditingStarted() override
    {
        SimulationThreadsController::data_synchronizer.lock();
    }

    void OnEditingEnded() override
    {
        SimulationThreadsController::d_udelta = 1000000 / SimulationThreadsController::fps_limit;
        SimulationThreadsController::data_synchronizer.unlock();
    }
} STCManager;

SimulationThreadsController::SimulationThreadsController() {}

void SimulationThreadsController::WidenInspectorContext()
{
    Inspector::AddHeader("simulation", LARGE_HEADER);
    Inspector::AddParam("physics dt[mcs]", p_udelta, 0, 1000000);
    Inspector::AddParam("FPS limit", fps_limit, 1, 1000);
}

EditingManager *SimulationThreadsController::GetEditingManager()
{
    return &STCManager;
}

void SimulationThreadsController::Mount(SoftScene *scene, unsigned long physics_udelta, QWidget *scene_view, unsigned long drawing_udelta)
{
    QMutexLocker data_lock(&data_synchronizer);
    QMutexLocker threads_lock(&threads_synchronizer);

    SimulationThreadsController::scene = scene;
    p_udelta = physics_udelta;
    p_thread = nullptr;

    SimulationThreadsController::scene_view = scene_view;
    d_udelta = drawing_udelta;
    d_thread = nullptr;
    fps_limit = 1000000 / d_udelta;
}

SoftScene *SimulationThreadsController::GetScene()
{
    QMutexLocker lock(&data_synchronizer);

    return scene;
}

QWidget *SimulationThreadsController::GetSceneView()
{
    QMutexLocker lock(&data_synchronizer);

    return scene_view;
}

unsigned long SimulationThreadsController::GetPhysicsDelta()
{
    QMutexLocker lock(&data_synchronizer);

    return p_udelta;
}

void SimulationThreadsController::SetPhysicsDelta(unsigned long udelta)
{
    QMutexLocker lock(&data_synchronizer);

    p_udelta = udelta;
}

unsigned long SimulationThreadsController::GetDrawingDelta()
{
    QMutexLocker lock(&data_synchronizer);

    return d_udelta;
}

void SimulationThreadsController::SetDrawingDelta(unsigned long udelta)
{
    QMutexLocker lock(&data_synchronizer);

    d_udelta = udelta;
}

void SimulationThreadsController::Run()
{
    QMutexLocker lock(&threads_synchronizer);

    if(!p_thread)
    {
        p_thread = new PhysicalThread();
        p_thread->start();
    }

    if(!d_thread)
    {
        d_thread = new DrawingThread();
        CONNECT(d_thread, &DrawingThread::Draw, scene_view, qOverload<>(&QWidget::update));
        d_thread->start();
    }
}

void SimulationThreadsController::Stop()
{
    QMutexLocker lock(&threads_synchronizer);

    if(p_thread)
        delete p_thread;
    p_thread = nullptr;

    if(d_thread)
        delete d_thread;
    d_thread = nullptr;
}
