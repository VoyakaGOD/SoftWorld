#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include "sceneview.h"

class PalleteItem : public QWidget
{
    Q_OBJECT

    public:
        explicit PalleteItem(QWidget *parent, SceneView *sceneview, PhysicalBody *body, QString str);
        QString name;

    protected:
        SceneView* scene_view_target;
        PhysicalBody* body;


        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
};
