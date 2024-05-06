#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action, EditingManager *manager)
{
    button = new QPushButton(name, container);
    layout->addRow(button);
    CONNECT(button, &QPushButton::clicked, [=]() {
        if(!manager->PrepareForEditing())
            return;
        action();
        manager->EndEditing();
    });
}

InspectorButton::~InspectorButton()
{
    //button->disconnect(button, nullptr, nullptr, nullptr);
    delete button;
}
