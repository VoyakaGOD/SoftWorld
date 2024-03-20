#pragma once

#include <vector>
#include <QWidget>
#include <QLayout>
#include "palleteitem.h"

class Pallete : public QFrame {
    Q_OBJECT

    SceneView* sceneview = nullptr;
    PalleteItem* context_menu_target = nullptr;
    QAction delete_action;

    public:
    QVBoxLayout layout;

    explicit Pallete(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    ~Pallete();

    void PostInit(SceneView* view);

    void AddPalleteItem(PhysicalBody* body, QString name = "");

    int FindItemByPos(const QPoint &pos);

    public slots:
        void ShowContextMenu(const QPoint &pos);
        void RemoveThisPalleteItem();
};


