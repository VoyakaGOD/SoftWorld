#ifndef DRAWINGTHREAD_H
#define DRAWINGTHREAD_H

#include <QThread>

class DrawingThread: public QThread
{
    Q_OBJECT
private:
    bool is_running;

public:
    DrawingThread();
    ~DrawingThread();
    void run() override;

signals:
    void Draw();
};

#endif // DRAWINGTHREAD_H
