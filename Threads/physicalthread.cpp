#include "physicalthread.h"
#include "simulationthreadscontroller.h"

PhysicalThread::PhysicalThread() : is_running(true) {}

PhysicalThread::~PhysicalThread()
{
    is_running = false;
    wait();
}

void PhysicalThread::run()
{
    while(is_running)
    {
        SoftScene *scene = SimulationThreadsController::GetScene();
        unsigned long udelta = SimulationThreadsController::GetPhysicsDelta();

        scene->DoNextStep(udelta);
        usleep(udelta);
    }
}
