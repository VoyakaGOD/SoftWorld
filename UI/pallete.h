#pragma once

#include <vector>
#include <QWidget>
#include <QLayout>
#include "palleteitem.h"

class Pallete : QLayout {
    Q_OBJECT
    void AddPalleteItem(PalleteItem* item) {
        item->setMinimumHeight(32);
        item->setMaximumHeight(32);
        this->addWidget(item);
    }
    void RemovePalleteItem(PalleteItem* item) {
        this->removeWidget(item);
    }
}
