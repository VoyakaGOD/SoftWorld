#ifndef PALLETE_H
#define PALLETE_H

#include <vector>
#include <QWidget>
#include <QLayout>
#include "palleteitem.h"
#include "Inspector/editingmanager.h"

class Pallete : public QFrame, public EditingManager, public SerializableObject {
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

    void Clear();
    PalleteItem* AddPalleteItem(PalleteItem* item);
    PalleteItem* AddPalleteItem(PhysicalBody* body, QString name = QString());
    void RemovePalleteItem(PalleteItem* item);

    int FindItemByPos(const QPoint &pos);

    // fileworks
    virtual void SaveID(DataStorageWriter &data) const override;
    virtual size_t GetSavedSize() const override;
    virtual void SaveData(DataStorageWriter &data) const override;

    public slots:
        void ShowContextMenu(const QPoint &pos);
        void RemoveThisPalleteItem();
        void InspectThisPalleteItem();

        void SaveThisPalleteItem();
        void LoadNewPalleteItem();
        void PickItemFromScene();

        bool PrepareForEditing() override;
        void EndEditing() override;
};

#endif
