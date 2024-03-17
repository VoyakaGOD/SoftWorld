#pragma once

#include <vector>
#include <QWidget>
#include <QLayout>
#include "palleteitem.h"

class Pallete : public QWidget {
    Q_OBJECT

    SceneView* sceneview;
    PalleteItem* context_menu_target = nullptr;

    public:
    QVBoxLayout layout;

    explicit Pallete(QWidget *parent = nullptr);

    void PostInit(SceneView* view);

    void AddPalleteItem(PhysicalBody* body, QString name = "");

    int FindItemByPos(const QPoint &pos);

    public slots:
        void ShowContextMenu(const QPoint &pos);
        void RemoveThisPalleteItem();
};


