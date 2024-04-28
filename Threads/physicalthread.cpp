#include "physicalthread.h"
#include "simulationthreadscontroller.h"

PhysicalThread::PhysicalThread()
{
    sleepy.lock();
}

PhysicalThread::~PhysicalThread()
{
    sleepy.unlock();
    wait();
}

void PhysicalThread::run()
{
    while(true)
    {
        SoftScene *scene = SimulationThreadsController::GetScene();
        unsigned long udelta = SimulationThreadsController::GetPhysicsDelta();

        scene->DoNextStep(udelta * 1e-6d);
        if(sleepy.tryLock(udelta / 1000))
            break;
    }
    sleepy.unlock();
}
