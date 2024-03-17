#ifndef INSPECTORINTEGERFIELD_H
#define INSPECTORINTEGERFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QFormLayout>
#include <QSlider>
#include "inspectableparam.h"
#include "inspectoritem.h"

template <typename T>
class InspectorIntegerField : public InspectorItem
{
private:
    QLabel *label;
    QSpinBox *box;
    QSlider *slider;
    QHBoxLayout *field;
public:
    InspectorIntegerField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<T> &param)
    {
        label = new QLabel(param.name, container);

        box = new QSpinBox(container);
        box->setMaximum(static_cast<int>(param.max));
        box->setMinimum(static_cast<int>(param.min));
        box->setContextMenuPolicy(Qt::PreventContextMenu);

        slider = new QSlider(Qt::Horizontal, container);
        slider->setMaximum(static_cast<int>(param.max));
        slider->setMinimum(static_cast<int>(param.min));

        field = new QHBoxLayout();
        field->addWidget(box, 25);
        field->addWidget(slider, 75);
        field->setSpacing(7);
        layout->addRow(label, field);

        CONNECT(box, &QSpinBox::valueChanged, [=](int value){ param.value = value; UpdateValue(value); });
        CONNECT(slider, &QSlider::valueChanged, [=](int value){ param.value = value; UpdateValue(value); });
    }

    ~InspectorIntegerField()
    {
        delete label;
        delete box;
        delete slider;
        delete field;
    }
private:
    void UpdateValue(int value)
    {
        box->setValue(value);
        slider->setValue(value);
    }
};

#endif // INSPECTORINTEGERFIELD_H
