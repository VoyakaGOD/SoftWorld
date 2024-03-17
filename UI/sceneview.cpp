#include "sceneview.h"
#include <QMouseEvent>
#include <iostream>

extern SoftScene main_scene;

SceneView::SceneView(QWidget *parent)
    : QWidget(parent), scene(&main_scene), selected_body(nullptr) {
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

    if (this->inserted_body){
        QPoint mouse_pos = ToSceneCoordinates(this->mapFromGlobal(QCursor::pos()));
        painter.translate(mouse_pos);
        this->inserted_body->Draw(painter);
    }
}

QPoint SceneView::ToSceneCoordinates(QPoint point) {
    return QPoint(
        (point.x() - this->translation.x())/(this->xscale),
        (point.y() - this->translation.y())/(this->yscale)
    );
}

void SceneView::mousePressEvent(QMouseEvent *event) {
    QPoint pos = this->ToSceneCoordinates(event->pos());

    if (inserted_body) {
        PhysicalBody* new_body = this->inserted_body->Clone();
        new_body->MoveBy(event->pos());
        this->scene->AddBody(new_body);
        this->inserted_body = nullptr;
    }

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
    if (body_grabbed) {
        QPoint pos = this->ToSceneCoordinates(event->pos());
        QPoint offset = pos - this->selected_body->GetGlobalCoordinate(this->grabded_point);
        this->selected_body->MoveBy(offset);
        this->update();
    }
    if (inserted_body) {
        this->update();
    }
}

void SceneView::SetInsertion(PhysicalBody* body) {
    this->inserted_body = body;
}
