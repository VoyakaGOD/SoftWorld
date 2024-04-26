#ifndef PHYSICALTHREAD_H
#define PHYSICALTHREAD_H

#include <QThread>
#include "Physics/softscene.h"

class PhysicalThread : public QThread
{
    Q_OBJECT
private:
    QMutex sleepy;

public:
    PhysicalThread();
    ~PhysicalThread();
    void run() override;
};

#endif // PHYSICALTHREAD_H
