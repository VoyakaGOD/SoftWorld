#include "sceneview.h"
#include <QMouseEvent>
#include <iostream>

extern SoftScene main_scene;

SceneView::SceneView(QWidget *parent)
    : QWidget(parent), scene(&main_scene) {
    //this->SetViewport(this->scene->world_rect);
}

void SceneView::SetViewport(QRect &rect) {
    this->translation = -rect.topLeft();
    this->xscale = this->width() / rect.width();
    this->yscale = this->height() / rect.height();
}

void SceneView::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.scale(this->xscale, this->yscale);
    painter.translate(this->translation);
    this->scene->Draw(painter);
}

QPoint SceneView::ToSceneCoordinates(QPoint point) {
    return QPoint(
        (point.x() - this->translation.x())/(this->xscale),
        (point.y() - this->translation.y())/(this->yscale)
    );
}

void SceneView::mousePressEvent(QMouseEvent *event) {
    QPoint pos = this->ToSceneCoordinates(event->pos());
    PhysicalBody* body = this->scene->GetBodyAt(pos);
    if (body) {
        this->selected_body = body;
        this->body_grabbed = true;
        this->grabded_point = body->GetLocalCoordinate(pos);
    }
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
    if (body_grabbed) {
        body_grabbed = false;
    }
}

void SceneView::mouseMoveEvent(QMouseEvent *event) {
    /*if (body_grabbed) {
        QPoint pos = this->ToSceneCoordinates(event->pos());
        QPoint offset = pos - this->selected_body->GetGlobalCoordinate(this->grabded_point);
        this->selected_body->MoveBy(offset);
        this->update();
    }*/

}
