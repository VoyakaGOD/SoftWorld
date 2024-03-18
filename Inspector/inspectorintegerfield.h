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
        int max_value = static_cast<int>(param.max);
        int min_value = static_cast<int>(param.min);

        label = new QLabel(param.name, container);

        box = new QSpinBox(container);
        box->setMaximum(max_value);
        box->setMinimum(min_value);
        box->setContextMenuPolicy(Qt::PreventContextMenu);

        slider = new QSlider(Qt::Horizontal, container);
        slider->setMaximum(max_value);
        slider->setMinimum(min_value);

        field = new QHBoxLayout();
        field->addWidget(box, 25);
        field->addWidget(slider, 75);
        field->setSpacing(7);
        layout->addRow(label, field);

        CONNECT(box, &QSpinBox::valueChanged, [=](int value){ param.value = value; UpdateValue(value); });
        CONNECT(slider, &QSlider::valueChanged, [=](int value){ param.value = value; UpdateValue(value); });

        param.value = qBound(min_value, param.value, max_value);
        UpdateValue(param.value);
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
