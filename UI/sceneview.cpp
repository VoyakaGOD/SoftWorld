#include "sceneview.h"
#include <QMouseEvent>
#include <iostream>

#include "palleteitem.h" // TODO remove

extern SoftScene main_scene;

SceneView::SceneView(QWidget *parent)
    : QFrame(parent), scene(&main_scene),
    delete_action("delete selected body", this), clear_cursor_action("clear cursor", this) {

    this->delete_action.setShortcut(QKeySequence::Delete);
    this->delete_action.setShortcutContext(Qt::WidgetShortcut);
    connect(&(this->delete_action), SIGNAL(triggered()), this, SLOT(DeleteSelected()));
    this->addAction(&(this->delete_action));

    this->clear_cursor_action.setShortcut(QKeySequence("q"));
    this->clear_cursor_action.setShortcutContext(Qt::WidgetShortcut);
    connect(&(this->clear_cursor_action), SIGNAL(triggered()), this, SLOT(ClearCursor()));
    this->addAction(&(this->clear_cursor_action));

    this->setFrameStyle(QFrame::Panel | QFrame::Raised);
    this->setLineWidth(2);
    //this->SetViewport(this->scene->world_rect);
}

SceneView::~SceneView() {
    this->ClearCursor();
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

    if (this->selected_body && Inspector::IsTarget(this->selected_body)){
        painter.setPen(QPen(QBrush(this->palette().highlight()), 1, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(this->selected_body->GetBoundingRect());
    }

    if (this->inserted_body){
        QPoint mouse_pos = ToSceneCoordinates(this->mapFromGlobal(QCursor::pos()));
        painter.translate(mouse_pos);
        this->inserted_body->Draw(painter);
    }

    QFrame::paintEvent(event);
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
        this->ClearCursor();
    }

    PhysicalBody* body = this->scene->GetBodyAt(pos);
    if (body) {
        this->selected_body = body;
        this->body_grabbed = true;
        this->grabded_point = body->GetLocalCoordinate(pos);
        Inspector::SetTarget(body, this);
        body->WidenInspectorContext();
    }
    else
    {
        Inspector::SetTarget(this, this);
        this->scene->WidenInspectorContext();
        Inspector::SetManager(SimulationThreadsController::GetEditingManager());
        SimulationThreadsController::WidenInspectorContext();
    }
    this->update();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
    if (body_grabbed) {
        body_grabbed = false;
    }
    this->update();
}

void SceneView::mouseMoveEvent(QMouseEvent *event) {
    if (body_grabbed) {
        // TODO mutex here
        QPoint pos = this->ToSceneCoordinates(event->pos());
        QPoint offset = pos - this->selected_body->GetGlobalCoordinate(this->grabded_point);
        this->selected_body->MoveBy(offset);
        this->update();
    }
    if (inserted_body) {
        this->setFocus();
        this->update();
    }
}

void SceneView::SetInsertion(PhysicalBody* body) {
    if (this->inserted_body_pi) {
        this->inserted_body_pi->SetInactive();
        this->inserted_body_pi = nullptr;
    }
    this->inserted_body = body;
    this->update();
}

void SceneView::SetInsertion(PalleteItem* item) {
    this->SetInsertion(item->body);
    this->inserted_body_pi = item;
}

void SceneView::ClearCursor() {
    this->SetInsertion((PhysicalBody*)nullptr);
}

void SceneView::DeleteSelected() {
    if (this->selected_body) {
        this->body_grabbed = false;
        this->scene->RemoveBody(this->selected_body);
        delete this->selected_body;
        this->selected_body = nullptr;
        this->update();
    }
}

bool SceneView::PrepareForEditing()
{
    //try to lock
    scene->Lock();
    return true; //if try is successful
}

void SceneView::EndEditing()
{
    scene->Unlock();
    this->update();
}
