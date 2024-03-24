#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <Physics/softscene.h>
#include "Inspector/editingmanager.h"

class PalleteItem; // TODO find better way

class SceneView : public QWidget, public EditingManager
{
    Q_OBJECT

    public:
        QPoint translation = QPoint();
        qreal xscale = 1;
        qreal yscale = 1;
        SoftScene* scene = NULL;

        PhysicalBody* selected_body = nullptr;
        bool body_grabbed = false;
        QPoint grabded_point = QPoint();

        PhysicalBody* inserted_body = nullptr;
        PalleteItem* inserted_body_pi = nullptr;

        explicit SceneView(QWidget *parent = nullptr);
        ~SceneView();

        void SetViewport(QRect &rect);
        void SetInsertion(PhysicalBody* body);
        void SetInsertion(PalleteItem* item);
        QPoint ToSceneCoordinates(QPoint point);

        void OnEditingStarted() override;
        void OnEditingEnded() override;

    public slots:
        void ClearCursor();
        void DeleteSelected();

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;

        QAction delete_action;
        QAction clear_cursor_action;
};
