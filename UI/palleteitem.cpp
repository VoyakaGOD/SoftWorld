#include "palleteitem.h"
#include <iostream>
#include <QStyle>
#include <QMouseEvent>

PalleteItem::PalleteItem(QWidget *parent, SceneView *scene_view, PhysicalBody* body, QString str)
    : QWidget(parent), scene_view_target(scene_view), body(body), name(str) {
        this->setMaximumHeight(32);
        this->setMinimumHeight(32);
        this->setFocusPolicy(Qt::ClickFocus);
    }

PalleteItem::~PalleteItem() {
    if (this->body) {
        if (this->scene_view_target && this->scene_view_target->inserted_body == this->body) {
            this->scene_view_target->SetInsertion(nullptr);
        }
        delete this->body;
    }
}

void PalleteItem::paintEvent(QPaintEvent *event) {
    int height = this->size().height();
    if (height > 30) {
        height = 30;
    }
    QPainter painter(this);
    this->style()->drawItemText(&painter,
        this->rect().adjusted(height, 0, 0, 0), 0, this->palette()
        , 1, this->name);

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

}

void PalleteItem::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        this->scene_view_target->SetInsertion(this->body);
}
