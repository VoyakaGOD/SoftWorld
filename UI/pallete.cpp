#include "pallete.h"
#include <QMenu>
#include <QModelIndex>
#include <Physics/ghostbody.h> //temporary
#include <iostream>

Pallete::Pallete(QWidget *parent)
 : QWidget(parent), layout(this) {
    this->setLayout(&(this->layout));
    //this->setLayoutDirection(Qt::RightToLeft);
    this->layout.addSpacerItem(new QSpacerItem(0,0, QSizePolicy::Preferred, QSizePolicy::Expanding));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(ShowContextMenu(const QPoint &)));
}

void Pallete::PostInit(SceneView* view) {
    this->sceneview = view;
    this->AddPalleteItem(new GhostBody(), "test");
    this->AddPalleteItem(new GhostBody(), "test2");
}

void Pallete::AddPalleteItem(PhysicalBody* body, QString name) {
    PalleteItem* item = new PalleteItem(dynamic_cast<QWidget*>(this->parent()), this->sceneview, body, name);
    this->layout.insertWidget(this->layout.count()-1, item);
    cout << item->pos().x() << "_" << item->pos().y() << "_" << item->height() << "_" << item->width() << endl;
}

void Pallete::RemoveThisPalleteItem() {
    if (context_menu_target) {
        this->layout.removeWidget(this->context_menu_target);
        delete this->context_menu_target;
        this->context_menu_target = nullptr;
    }
}

void Pallete::ShowContextMenu(const QPoint &pos){

    this->context_menu_target = dynamic_cast<PalleteItem*>(this->childAt(pos));

    QMenu *menu=new QMenu(static_cast<QWidget*>(this));
    if (this->context_menu_target) {
        QAction *item_del_act = new QAction("delete", this);
        item_del_act->connect(item_del_act, SIGNAL(triggered()) , this, SLOT(RemoveThisPalleteItem()));
        menu->addAction(item_del_act);
    }
    menu->popup(((QWidget*)this)->mapToGlobal(pos));
}
