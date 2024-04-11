#ifndef SIMULATIONTHREADSCONTROLLER_H
#define SIMULATIONTHREADSCONTROLLER_H

#include "physicalthread.h"

//udelta in microseconds
class SimulationThreadsController
{
private:
    PhysicalThread *p_thread;
public:
    static void Run();
    static void Stop();
    static void SetPhysicalDelta(unsigned long udelta);
    static void SetFPS(float fps);

private:
    SimulationThreadsController();
};

#endif // SIMULATIONTHREADSCONTROLLER_H
