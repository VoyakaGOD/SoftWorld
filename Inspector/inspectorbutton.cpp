#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const char *name, Action action)
{
    button = new QPushButton(name, container);
    layout->addRow(button);
    CONNECT(button, &QPushButton::clicked, action);
}

InspectorButton::~InspectorButton()
{
    delete button;
}
