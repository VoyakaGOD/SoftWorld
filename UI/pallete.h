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
    QAction inspect_action;
    QAction save_item_action;
    QAction load_item_action;
    QAction pick_item_action;

    public:
    QVBoxLayout layout;

    explicit Pallete(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    ~Pallete();

    void PostInit(SceneView* view);
    PalleteItem* AddPalleteItem(PalleteItem* item);
    PalleteItem* AddPalleteItem(PhysicalBody* body, QString name = "");
    void RemovePalleteItem(PalleteItem* item);

    int FindItemByPos(const QPoint &pos);

    public slots:
        void ShowContextMenu(const QPoint &pos);
        void RemoveThisPalleteItem();
        void InspectThisPalleteItem();

        void SaveThisPalleteItem();
        void LoadNewPalleteItem();
        void PickItemFromScene();
};


