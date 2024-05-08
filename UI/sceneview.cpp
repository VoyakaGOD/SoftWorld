#include "sceneview.h"
#include <QMouseEvent>
#include <iostream>

#include "palleteitem.h" // TODO remove

static const int scene_border_linewidth = 4;
static const int max_yeet_delta_time = 100;

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
    this->SetViewport(this->scene->world_rect);
}

SceneView::~SceneView() {
    this->ClearCursor();
}

void SceneView::SetViewport(QRect &rect) {
    this->viewport = rect;
    this->UpdateScaling();
}

void SceneView::UpdateScaling() {
    if (this->preserve_aspect_ratio) {
        double scale = min(((double)(this->width())) / this->viewport.width(), ((double)(this->height())) / this->viewport.height());
        this->xscale = scale;
        this->yscale = scale;
    }
    else {
        this->xscale = this->width() / this->viewport.width();
        this->yscale = this->height() / this->viewport.height();
    }

    this->translation = QPoint(this->width() / 2 / this->xscale, this->height() / 2 / this->yscale) - this->viewport.center();


    /*
    this->setFrameRect(QRect((this->viewport.left() + this->translation.x()) / this->xscale,
                             (this->viewport.top()  + this->translation.y()) / this->yscale,
                             (this->viewport.width() / this->xscale ),
                             (this->viewport.height() / this->yscale)));
    */

}

void SceneView::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.scale(this->xscale, this->yscale);
    painter.translate(this->translation);
    this->scene->Draw(painter);

    painter.setPen(QPen(Qt::black, scene_border_linewidth));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(this->scene->world_rect);

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
        (point.x()/this->xscale) - this->translation.x(),
        (point.y()/this->yscale) - this->translation.y()
    );
}

void SceneView::mousePressEvent(QMouseEvent *event) {
    QPoint pos = this->ToSceneCoordinates(event->pos());

    if (inserted_body) {
        PhysicalBody* new_body = this->inserted_body->Clone();
        new_body->MoveBy(pos);
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
        this->selected_body = nullptr;
    }
    this->update();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
    if (body_grabbed) {
        uint64_t delta_time = event->timestamp() - this->last_move_time_ms;
        if (delta_time < max_yeet_delta_time) {
            this->selected_body->AddMomentum(scene->GetThrowingScale() * last_mouse_speed - this->selected_body->GetCenterVelocity());
        }
        else {
            this->selected_body->AddMomentum(-this->selected_body->GetCenterVelocity());
        }
        this->body_grabbed = false;
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

        uint64_t delta_time = event->timestamp() - this->last_move_time_ms;
        if (delta_time > 0)
            this->last_mouse_speed = (500 * QVector2D(offset) / delta_time) + this->last_mouse_speed / 2;

        this->last_move_time_ms = event->timestamp();
    }
    if (inserted_body) {
        this->setFocus();
        this->update();
    }
}

void SceneView::resizeEvent(QResizeEvent* event) {
    this->UpdateScaling();
    QFrame::resizeEvent(event);
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
        if (Inspector::IsTarget(this->selected_body)) {
            Inspector::Clear();
        }
        this->selected_body = nullptr;
        this->update();
    }
}

bool SceneView::PrepareForEditing()
{
    return scene->TryToLock();
}

void SceneView::EndEditing()
{
    scene->Unlock();
    this->update();
}
