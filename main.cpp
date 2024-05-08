#include "mainwindow.h"
#include <Physics/softscene.h>

#include <QApplication>

SoftScene main_scene = SoftScene(QRect(0,0,500,500), 1.22, 10, 1);
MainWindow* main_window = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    main_window = &w;

    w.show();
    return a.exec();
}
