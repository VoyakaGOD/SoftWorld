#ifndef INSPECTORBUTTON_H
#define INSPECTORBUTTON_H

#include <QPushButton>
#include <QFormLayout>
#include "inspectableaction.h"
#include "inspectoritem.h"

class InspectorButton : public InspectorItem
{
private:
    QPushButton *button;
public:
    InspectorButton(QWidget *container, QFormLayout *layout, const InspectableAction &action);
    ~InspectorButton();
};

#endif // INSPECTORBUTTON_H
