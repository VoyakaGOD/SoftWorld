#include "drawingthread.h"
#include "simulationthreadscontroller.h"

DrawingThread::DrawingThread()
{
    sleepy.lock();
}

DrawingThread::~DrawingThread()
{
    disconnect();
    sleepy.unlock();
    wait();
}

void DrawingThread::run()
{
    while(true)
    {
        unsigned long udelta = SimulationThreadsController::GetPhysicsDelta();

        emit Draw();
        if(sleepy.tryLock(udelta / 1000))
            break;
    }
    sleepy.unlock();
}

