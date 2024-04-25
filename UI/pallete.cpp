#include "pallete.h"
#include <QMenu>
#include <QModelIndex>
#include <Physics/ghostbody.h> //temporary
#include <Utils/serialize.h>
#include <QFileDialog>
#include <iostream>
#include <Inspector/inspector.h>

Pallete::Pallete(QWidget *parent, Qt::WindowFlags f)
 : QFrame(parent, f), layout(this),
   delete_action("delete", this), inspect_action("inspect", this),
   load_item_action("add body from file", this), save_item_action("save body", this),
   pick_item_action("add selected body", this) {
    this->setLayout(&(this->layout));
    //this->setLayoutDirection(Qt::RightToLeft);
    this->layout.addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setFrameStyle(QFrame::Panel | QFrame::Raised);
    this->setLineWidth(2);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(ShowContextMenu(const QPoint &)));

    connect(&(this->delete_action)    , SIGNAL(triggered()) , this, SLOT(RemoveThisPalleteItem()));
    connect(&(this->inspect_action)   , SIGNAL(triggered()) , this, SLOT(InspectThisPalleteItem()));
    connect(&(this->save_item_action) , SIGNAL(triggered()) , this, SLOT(SaveThisPalleteItem()));

    connect(&(this->load_item_action) , SIGNAL(triggered()) , this, SLOT(LoadNewPalleteItem()));
    connect(&(this->pick_item_action) , SIGNAL(triggered()) , this, SLOT(PickItemFromScene()));
}

Pallete::~Pallete() {
    while (this->layout.count() > 0) {
        QLayoutItem* item = this->layout.itemAt(0);
        QWidget* widget = item->widget();
        if (widget) {
            this->layout.removeWidget(widget);
            PalleteItem* pi = dynamic_cast<PalleteItem*>(widget);
            delete widget;
            continue;
        }
        this->layout.removeItem(item);
        delete item;
    }
}


void Pallete::PostInit(SceneView* view) {
    this->sceneview = view;
    this->AddPalleteItem(new GhostBody(), "test");
    this->AddPalleteItem(new GhostBody(), "test2");
}

PalleteItem* Pallete::AddPalleteItem(PhysicalBody* body, QString name) {
    PalleteItem* item = new PalleteItem(this, Qt::WindowFlags(), this->sceneview, body, name);
    this->layout.insertWidget(this->layout.count()-1, item);
    return item;
}

PalleteItem* Pallete::AddPalleteItem(PalleteItem* item) {
    this->layout.insertWidget(this->layout.count()-1, item);
    return item;
}

void Pallete::RemovePalleteItem(PalleteItem* item) {
    this->layout.removeWidget(item);
    delete item;
}

void Pallete::RemoveThisPalleteItem() {
    if (this->context_menu_target) {
        this->RemovePalleteItem(this->context_menu_target);
        this->context_menu_target = nullptr;
    }
}

void Pallete::InspectThisPalleteItem() {
    if (this->context_menu_target) {
        Inspector::SetTarget(context_menu_target->body, this);
        this->context_menu_target->body->WidenInspectorContext();
    }
}

void Pallete::SaveThisPalleteItem() {
    if (this->context_menu_target) {
        try {
            QString filename =QFileDialog::getSaveFileName(this, "Open file");
            if (filename.isNull()) return;

            FileWriteInterface file_writer = FileWriteInterface(filename.toUtf8().data());
            SERIALIZE_OBJ(file_writer, *(this->context_menu_target))
        }
        catch (system_error err) {
            cout << err.what() << endl;
        }
    }
}

void Pallete::LoadNewPalleteItem() {
    try {
        QString filename =QFileDialog::getOpenFileName(this, "Open file");
        if (filename.isNull()) return;

        FileReadInterface file_reader = FileReadInterface(filename.toUtf8().data());
        this->AddPalleteItem(palleteItemDeserialize(this, this->windowFlags(), this->sceneview, file_reader));
    }
    catch (system_error err) {
        cout << err.what() << endl;
    }
}

void Pallete::PickItemFromScene() {
    PhysicalBody* new_body = this->sceneview->selected_body->Clone();

    new_body->MoveBy(new_body->GetBoundingRect().center() * -1);
    this->AddPalleteItem(new_body, "New body");
}

void Pallete::ShowContextMenu(const QPoint &pos){

    this->context_menu_target = dynamic_cast<PalleteItem*>(this->childAt(pos));

    QMenu *menu=new QMenu(static_cast<QWidget*>(this));
    if (this->context_menu_target) {
        menu->addAction(&(this->delete_action));
        menu->addAction(&(this->inspect_action));
        menu->addAction(&(this->save_item_action));
    }
    menu->addAction(&(this->load_item_action));
    menu->addAction(&(this->pick_item_action));
    menu->popup(((QWidget*)this)->mapToGlobal(pos));
}

bool Pallete::PrepareForEditing()
{
    return true;
}

void Pallete::EndEditing()
{
    context_menu_target->update();
}
