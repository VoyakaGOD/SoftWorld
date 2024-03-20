#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include "sceneview.h"

class PalleteItem : public QFrame
{
    Q_OBJECT

    public:
        explicit PalleteItem(QWidget *parent, Qt::WindowFlags f, SceneView *sceneview, PhysicalBody *body, QString str);
        ~PalleteItem();

        QString name;
        SceneView* scene_view_target;
        PhysicalBody* body;

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
};
