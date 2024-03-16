#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const InspectableAction action)
{
    button = new QPushButton(action.name, container);
    layout->addRow(button);
    this->action = action.action;
    connect(button, SIGNAL(clicked()), this, SLOT(HandleClick()));
}

InspectorButton::~InspectorButton()
{
    delete button;
}

void InspectorButton::HandleClick()
{
    action();
}
