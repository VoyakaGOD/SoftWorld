#ifndef INSPECTORBUTTON_H
#define INSPECTORBUTTON_H

#include <QPushButton>
#include <QFormLayout>
#include "inspectableaction.h"
#include "inspectoritem.h"

class InspectorButton : public QObject, public InspectorItem
{
    Q_OBJECT
private:
    QPushButton *button;
    Action action;
private slots:
    void HandleClick();
public:
    InspectorButton(QWidget *container, QFormLayout *layout, const InspectableAction &action);
    ~InspectorButton();
};

#endif // INSPECTORBUTTON_H
