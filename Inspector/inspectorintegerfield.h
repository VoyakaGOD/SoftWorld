#ifndef INSPECTORINTEGERFIELD_H
#define INSPECTORINTEGERFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include "inspectableparam.h"
#include "inspectoritem.h"

template <typename T>
class InspectorIntegerField : public InspectorItem
{
private:
    QLabel *label;
    QSpinBox *box;
    T &param_value;

public:
    InspectorIntegerField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<T> &param) : param_value(param.value)
    {
        label = new QLabel(param.name, container);
        box = new QSpinBox(container);
        box->setMaximum(static_cast<int>(param.max));
        box->setMinimum(static_cast<int>(param.min));
        box->setContextMenuPolicy(Qt::PreventContextMenu);
        layout->addRow(label, box);
    }

    ~InspectorIntegerField()
    {
        delete label;
        delete box;
    }
};

#endif // INSPECTORINTEGERFIELD_H
