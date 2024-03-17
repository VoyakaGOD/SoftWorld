#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const InspectableAction &action)
{
    button = new QPushButton(action.name, container);
    layout->addRow(button);
    CONNECT(button, &QPushButton::clicked, action.action);
}

InspectorButton::~InspectorButton()
{
    delete button;
}
