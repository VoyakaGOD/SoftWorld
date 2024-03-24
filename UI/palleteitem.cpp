#include "palleteitem.h"
#include <iostream>
#include <QStyle>
#include <QMouseEvent>
#include <Utils/serialize.h>
#include <QPalette>

static const int ITEM_MARGIN = 2;
static const int FRAME_WIDTH_BASE = 1;
static const int FRAME_WIDTH_SELECT = 2;

PalleteItem::PalleteItem(QWidget *parent, Qt::WindowFlags f, SceneView *scene_view, PhysicalBody* body, QString str)
    : QFrame(parent, f), scene_view_target(scene_view), body(body), name(str) {
        this->setMinimumHeight(32);
        this->setFocusPolicy(Qt::ClickFocus);
        this->setFrameStyle(QFrame::Panel | QFrame::Raised);
        this->setLineWidth(FRAME_WIDTH_BASE);
        this->setAutoFillBackground(true);
}

PalleteItem::~PalleteItem() {
    if (this->body) {
        if (this->scene_view_target) {
            if (this->scene_view_target->inserted_body == this->body) {
                this->scene_view_target->ClearCursor();
            }
        }
        delete this->body;
    }
}

PalleteItem::PalleteItem(QWidget *parent, Qt::WindowFlags f, SceneView *scene_view, DataStorageReader &data)
        : PalleteItem(parent, f, scene_view, nullptr, nullptr) {

    uint16_t fixed_len = *(uint16_t*)(data.data); // required "fixed data len" field
    PTR_MOVE_BYTES(data.data, fixed_len)
    this->body = physicalBodyDeserialize(data);
    char* name_str = stringDeserialize(data);
    this->name = name_str ? name_str : "*Unnamed*";
    DataObjectSkipEnd(data);
    PTR_MOVE_BYTES(data.data, 1)
}

void PalleteItem::SetActive() {
    this->setBackgroundRole(QPalette::Highlight);
    this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    this->setLineWidth(FRAME_WIDTH_SELECT);
}

void PalleteItem::SetInactive() {
    this->setBackgroundRole(QPalette::Window);
    this->setFrameStyle(QFrame::Panel | QFrame::Raised);
    this->setLineWidth(FRAME_WIDTH_BASE);
}

size_t PalleteItem::GetSavedSize() {
    return (2*sizeof(saved_obj_id_t)) + sizeof(obj_fixed_data_len_t) + (this->body ? this->body->GetSavedSize() : sizeof(saved_obj_id_t)) + (this->name.length() + 1 + sizeof(saved_obj_id_t));
}

void PalleteItem::SaveID(DataStorageWriter &data) {
    PTR_APPEND(data.data, saved_obj_id_t, SAVED_OBJ_PALLETE_ITEM)
}

void PalleteItem::SaveData(DataStorageWriter &data) {
    *(obj_fixed_data_len_t*)data.data = sizeof(obj_fixed_data_len_t);
    PTR_MOVE_BYTES(data.data, sizeof(obj_fixed_data_len_t))

    if (this->body)
        SAVE_OBJ(data, *(this->body))
    else {
        PTR_APPEND(data.data, saved_obj_id_t, SAVED_OBJ_INVALID)
    }

    size_t str_size = StringSerialize(this->name.toStdString(), data.data);
    PTR_MOVE_BYTES(data.data, str_size);
}


void PalleteItem::paintEvent(QPaintEvent *event) {
    int framewidth = this->frameWidth() + ITEM_MARGIN;
    int height = this->size().height() - 2*framewidth;
    if (height > 30) {
        height = 30;
    }
    QPainter painter(this);
    this->style()->drawItemText(&painter,
                                this->rect().adjusted(height + framewidth + ITEM_MARGIN, framewidth, -framewidth, -framewidth),
                                Qt::AlignHCenter, this->palette(), 1, this->name);

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
    if (event->button() == Qt::LeftButton) {
        if (this->scene_view_target->inserted_body_pi == this) {
            this->scene_view_target->ClearCursor();
        }
        else {
            this->SetActive();
            this->scene_view_target->SetInsertion(this);
        }
    }
}
