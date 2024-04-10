#include "physicalthread.h"

PhysicalThread::PhysicalThread(SoftScene &scene, unsigned long udelta) : scene(scene), udelta(udelta) {}

void PhysicalThread::run()
{
    while(true)
    {
        scene.DoNextStep(udelta);
        usleep(udelta);
    }
}

PhysicalThread::~PhysicalThread()
{
    thread()->wait();
    thread()->quit();
}

void PhysicalThread::SetDeltaTime(unsigned long udelta)
{
    this->udelta = udelta;
}
