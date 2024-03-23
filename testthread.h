#ifndef TESTTHREAD_H
#define TESTTHREAD_H

#include <QThread>
#include <QDebug>
#include <QWidget>

class TestThread : public QThread
{
    Q_OBJECT
private:
    QWidget *target;
public:
    TestThread(QWidget *target);
    void run() override;
public slots:
    void on_exit();
};

//temporary test thread

#endif // TESTTHREAD_H
