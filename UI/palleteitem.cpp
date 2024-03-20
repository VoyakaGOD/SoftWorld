#include "palleteitem.h"
#include <iostream>
#include <QStyle>
#include <QMouseEvent>

static const int ITEM_MARGIN = 2;

PalleteItem::PalleteItem(QWidget *parent, Qt::WindowFlags f, SceneView *scene_view, PhysicalBody* body, QString str)
    : QFrame(parent, f), scene_view_target(scene_view), body(body), name(str) {
        this->setMinimumHeight(32);
        this->setFocusPolicy(Qt::ClickFocus);
        this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    }

PalleteItem::~PalleteItem() {
    if (this->body) {
        if (this->scene_view_target) {
            if (this->scene_view_target->inserted_body == this->body) {
                this->scene_view_target->SetInsertion(nullptr);
            }
        }
        delete this->body;
    }
}

void PalleteItem::paintEvent(QPaintEvent *event) {
    int framewidth = this->frameWidth() + ITEM_MARGIN;
    int height = this->size().height() - 2*framewidth;
    if (height > 30) {
        height = 30;
    }
    QPainter painter(this);
    this->style()->drawItemText(&painter,
        this->rect().adjusted(height + framewidth + ITEM_MARGIN, framewidth, -framewidth, -framewidth), 0, this->palette()
        , 1, this->name);

    if (this->body) {
        QRect rect = this->body->GetBoundingRect();
        qreal scale = min(((qreal)height) / rect.height(), ((qreal)height) / rect.width());
        painter.scale(scale, scale);
        painter.translate(framewidth, framewidth);
        this->body->Draw(painter);
    }
    else {
        painter.setBrush(Qt::red);
        painter.drawRect(QRect(framewidth,framewidth,height,height));
    }

    QFrame::paintEvent(event);
}

void PalleteItem::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        this->scene_view_target->SetInsertion(this->body);
}
