#include "mainwindow.h"
#include <Physics/softscene.h>
#include <Physics/ghostbody.h>

#include <QApplication>

SoftScene main_scene = SoftScene(QRect(0,0,500,500), 0, 10);
MainWindow* main_window = NULL;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    main_window = &w;

    main_scene.AddBody(new GhostBody());

    w.show();
    return a.exec();
}
