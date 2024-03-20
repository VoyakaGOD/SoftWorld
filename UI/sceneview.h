#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QPainter>
#include <Physics/softscene.h>

class SceneView : public QWidget
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

        explicit SceneView(QWidget *parent = nullptr);

        void SetViewport(QRect &rect);
        void SetInsertion(PhysicalBody* body);
        QPoint ToSceneCoordinates(QPoint point);

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
