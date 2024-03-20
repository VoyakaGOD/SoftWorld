#include "pallete.h"
#include <QMenu>
#include <QModelIndex>
#include <Physics/ghostbody.h> //temporary
#include <Physics/serialize.h>
#include <iostream>

Pallete::Pallete(QWidget *parent, Qt::WindowFlags f)
 : QFrame(parent, f), layout(this),
   delete_action("delete", this), load_item_action("add body from file", this), save_item_action("save body", this) {
    this->setLayout(&(this->layout));
    //this->setLayoutDirection(Qt::RightToLeft);
    this->layout.addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    this->setFrameStyle(QFrame::Panel | QFrame::Raised);
    this->setLineWidth(2);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(ShowContextMenu(const QPoint &)));

    connect(&(this->delete_action)   , SIGNAL(triggered()) , this, SLOT(RemoveThisPalleteItem()));
    connect(&(this->load_item_action), SIGNAL(triggered()) , this, SLOT(LoadNewPalleteItem()));
    connect(&(this->save_item_action), SIGNAL(triggered()) , this, SLOT(SaveThisPalleteItem()));
}

Pallete::~Pallete() {
    while (this->layout.count() > 0) {
        QLayoutItem* item = this->layout.itemAt(0);
        QWidget* widget = item->widget();
        if (widget) {
            this->layout.removeWidget(widget);
            PalleteItem* pi = dynamic_cast<PalleteItem*>(widget);
            if (pi) { // TODO: this is not needed with proper destruction order and may cause UB when deleting an active pallete item
                pi->scene_view_target = nullptr;
            }
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

void Pallete::AddPalleteItem(PhysicalBody* body, QString name) {
    PalleteItem* item = new PalleteItem(this, Qt::WindowFlags(), this->sceneview, body, name);
    this->layout.insertWidget(this->layout.count()-1, item);
    cout << item->pos().x() << "_" << item->pos().y() << "_" << item->height() << "_" << item->width() << endl;
}

void Pallete::RemoveThisPalleteItem() {
    if (this->context_menu_target) {
        this->layout.removeWidget(this->context_menu_target);
        delete this->context_menu_target;
        this->context_menu_target = nullptr;
    }
}

void Pallete::SaveThisPalleteItem() {
    if (this->context_menu_target) {
        try {
            const char* filename = askFilename('w');
            if (!filename) return;
            printf("|%s|\n", filename);
            FileWriteInterface file_writer = FileWriteInterface(filename);
            bodySerialize(this->context_menu_target->body, file_writer);
        }
        catch (system_error err) {
            errno = err.code().value();
            perror(err.what());
        }
    }
}

void Pallete::LoadNewPalleteItem() {
    try {
        const char* filename = askFilename('r');
        if (!filename) return;
        FileReadInterface file_reader = FileReadInterface(filename);
        this->AddPalleteItem(bodyDeserialize(file_reader), "new item");
    }
    catch (system_error err) {
        errno = err.code().value();
        perror(err.what());
    }
}

void Pallete::ShowContextMenu(const QPoint &pos){

    this->context_menu_target = dynamic_cast<PalleteItem*>(this->childAt(pos));

    QMenu *menu=new QMenu(static_cast<QWidget*>(this));
    if (this->context_menu_target) {
        menu->addAction(&(this->delete_action));
        menu->addAction(&(this->save_item_action));
    }
    menu->addAction(&(this->load_item_action));
    menu->popup(((QWidget*)this)->mapToGlobal(pos));
}
