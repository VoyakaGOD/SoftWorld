#ifndef INSPECTORBUTTON_H
#define INSPECTORBUTTON_H

#include <QPushButton>
#include <QFormLayout>
#include "inspectoritem.h"
#include "action.h"

class InspectorButton : public InspectorItem
{
private:
    QPushButton *button;
public:
    InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action);
    ~InspectorButton();
};

#endif // INSPECTORBUTTON_H
