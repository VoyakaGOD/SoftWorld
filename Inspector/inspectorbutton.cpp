#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action, EditingManager *manager)
{
    button = new QPushButton(name, container);
    layout->addRow(button);
    CONNECT(button, &QPushButton::clicked, [=]() {
        manager->OnEditingStarted();
        action();
        manager->OnEditingEnded();
    });
}

InspectorButton::~InspectorButton()
{
    delete button;
}
