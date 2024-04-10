#ifndef PHYSICALTHREAD_H
#define PHYSICALTHREAD_H

#include <QThread>
#include "Physics/softscene.h"

//udelta in microseconds
class PhysicalThread : public QThread
{
    Q_OBJECT
private:
    SoftScene &scene;
    unsigned long udelta;

public:
    PhysicalThread(SoftScene &scene, unsigned long udelta);
    ~PhysicalThread();
    void run() override;

public slots:
    void SetDeltaTime(unsigned long udelta);
};

#endif // PHYSICALTHREAD_H
