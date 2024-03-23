#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action, LockableObject *scene)
{
    button = new QPushButton(name, container);
    layout->addRow(button);
    CONNECT(button, &QPushButton::clicked, [=]() {
        scene->Lock();
        action();
        scene->Unlock();
    });
}

InspectorButton::~InspectorButton()
{
    delete button;
}
