#include "palleteitem.h"
#include <iostream>


PalleteItem::PalleteItem(QWidget *parent, SceneView *scene_view, PhysicalBody* body, QString str)
    : QWidget(parent), scene_view_target(scene_view), body(body), name(str) {
        this->setMaximumHeight(32);
        this->setMinimumHeight(32);
    }

void PalleteItem::paintEvent(QPaintEvent *event) {
    int height = this->size().height();
    int pos = 0;
    if (height > 30) {
        height = 30;
    }
    QPainter painter(this);
    if (this->body) {
        QRect rect = this->body->GetBoundingRect();
        qreal scale = min(((qreal)height) / rect.height(), ((qreal)height) / rect.width());
        painter.scale(scale, scale);
        this->body->Draw(painter);
    }
    else {
        painter.setBrush(Qt::red);
        painter.drawRect(QRect(0,0,height,height));
    }
    pos += height;
    painter.drawText(QPoint(pos, height), this->name);
}

void PalleteItem::mousePressEvent(QMouseEvent *event) {
    this->scene_view_target->SetInsertion(this->body);
}
