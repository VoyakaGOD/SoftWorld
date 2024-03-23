#include "testthread.h"

TestThread::TestThread(QWidget *target)
{
    this->target = target;
}

void TestThread::run()
{
    while(1)
    {
        target->update();
        QThread::msleep(16);
    }
}

void TestThread::on_exit()
{
    thread()->quit();
    thread()->wait();
}
