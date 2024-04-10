#include "pallete.h"
#include <QMenu>
#include <QModelIndex>
#include <Physics/ghostbody.h> //temporary
#include <Serialize/serialize.h>
#include <Serialize/serialize_special.h>
#include <Serialize/deserialize.h>
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

void Pallete::Clear(){
    this->context_menu_target = nullptr;

    for (int i = 0, j = 0; i < this->layout.count(); i++) {
        QLayoutItem* item = this->layout.itemAt(j);
        PalleteItem* pi =  dynamic_cast<PalleteItem*>(item->widget());
        if (pi)
            this->RemovePalleteItem(pi);
        else
            j++;
    }
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
            serializeObj(file_writer, *(this->context_menu_target));
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
        PalleteItem* new_pi = this->AddPalleteItem(nullptr, QString());
        if (palleteItemDeserialize(new_pi, file_reader) != 0) {
            this->RemovePalleteItem(new_pi);
            //TODO: pop up error
        }

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

void Pallete::ShowContextMenu(const QPoint &pos) {

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

void Pallete::OnEditingStarted() {}

void Pallete::OnEditingEnded() {
    context_menu_target->update();
}

void Pallete::SaveID(DataStorageWriter &data) const {
}

size_t Pallete::GetSavedSize() const {
    int n = this->layout.count();
    size_t len = sizeof(saved_obj_id_t);
    int pi_n = 0;

    for (int i = 0; i < n; i++) {
        QLayoutItem* item = this->layout.itemAt(i);
        PalleteItem* pi =  dynamic_cast<PalleteItem*>(item->widget());
        if (pi) {
            pi_n++;
            len += pi->GetSavedSize();
        }
    }
    len += getPkgHeaderSize(pi_n);
    return len;
}

void Pallete::SaveData(DataStorageWriter &data) const {
    int n = this->layout.count();
    int pi_n = 0;
    for (int i = 0; i < n; i++) {
        QLayoutItem* item = this->layout.itemAt(i);
        PalleteItem* pi =  dynamic_cast<PalleteItem*>(item->widget());
        if (pi) {
            pi_n++; // maybe cringe
        }
    }

    savePkgHeader(pi_n, data);

    for (int i = 0; i < n; i++) {
        QLayoutItem* item = this->layout.itemAt(i);
        PalleteItem* pi =  dynamic_cast<PalleteItem*>(item->widget());
        if (pi) {
           saveObj(data, *pi);
        }
    }
}
