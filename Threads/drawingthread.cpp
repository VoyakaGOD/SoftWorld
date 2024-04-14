#include "drawingthread.h"
#include "simulationthreadscontroller.h"

DrawingThread::DrawingThread() : is_running(true) {}

DrawingThread::~DrawingThread()
{
    disconnect();
    is_running = false;
    wait();
}

void DrawingThread::run()
{
    while(is_running)
    {
        unsigned long udelta = SimulationThreadsController::GetPhysicsDelta();

        emit Draw();
        usleep(udelta);
    }
}

