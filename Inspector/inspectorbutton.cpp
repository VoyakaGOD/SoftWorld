#include "inspectorbutton.h"

InspectorButton::InspectorButton(QWidget *container, QFormLayout *layout, const InspectableAction action)
{
    QPushButton *button = new QPushButton(action.name, container);
    layout->addRow(button);
}
