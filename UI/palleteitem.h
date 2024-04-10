#ifndef PALLETEITEM_H
#define PALLETEITEM_H

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include "sceneview.h"
#include <Utils/fileworks.h>
#include <Serialize/deser_helpers.h>

///SERIALIZABLE PalleteItem SAVED_OBJ_PALLETE_ITEM inplace

class PalleteItem : public QFrame, public SerializableObject
{
    Q_OBJECT

    public:
        explicit PalleteItem(QWidget *parent, Qt::WindowFlags f, SceneView *sceneview, PhysicalBody *body, QString str);
        ~PalleteItem();

        QString name;
        SceneView* scene_view_target;
        PhysicalBody* body;

    public:
        static int Deserialize(SerializableObject* obj, DataStorageReader &data);
        virtual size_t GetSavedSize() const override;
        virtual void SaveID(DataStorageWriter &data) const override;
        virtual void SaveData(DataStorageWriter &data) const override;

        void SetActive();
        void SetInactive();

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
};

#endif
