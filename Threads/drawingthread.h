#ifndef DRAWINGTHREAD_H
#define DRAWINGTHREAD_H

#include <QThread>
#include <QMutex>

class DrawingThread: public QThread
{
    Q_OBJECT
private:
    QMutex sleepy;

public:
    DrawingThread();
    ~DrawingThread();
    void run() override;

signals:
    void Draw();
};

#endif // DRAWINGTHREAD_H
