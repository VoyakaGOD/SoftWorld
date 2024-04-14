#ifndef PHYSICALTHREAD_H
#define PHYSICALTHREAD_H

#include <QThread>
#include "Physics/softscene.h"

class PhysicalThread : public QThread
{
    Q_OBJECT
private:
    bool is_running;

public:
    PhysicalThread();
    ~PhysicalThread();
    void run() override;
};

#endif // PHYSICALTHREAD_H
