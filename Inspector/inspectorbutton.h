#ifndef INSPECTORBUTTON_H
#define INSPECTORBUTTON_H

#include <QPushButton>
#include <QFormLayout>
#include "inspectoritem.h"
#include "action.h"
#include "lockableobject.h"

class InspectorButton : public InspectorItem
{
private:
    QPushButton *button;
public:
    InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action, LockableObject *scene);
    ~InspectorButton();
};

#endif // INSPECTORBUTTON_H
