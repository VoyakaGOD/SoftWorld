#ifndef INSPECTORNUMERICFIELD_H
#define INSPECTORNUMERICFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
#include "inspectableparam.h"
#include "inspectoritem.h"
#include "slidermanagers.h"

template <typename T, typename SpinBoxT, typename ManagerT>
class InspectorNumericField : public InspectorItem
{
private:
    QLabel *label;
    SpinBoxT *box;
    QSlider *slider;
    QHBoxLayout *field;
    ManagerT manager;
public:
    InspectorNumericField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<T> &param)
    {
        T max_value = param.max;
        T min_value = param.min;

        label = new QLabel(param.name, container);

        box = new SpinBoxT(container);
        box->setMaximum(max_value);
        box->setMinimum(min_value);
        box->setContextMenuPolicy(Qt::PreventContextMenu);

        slider = new QSlider(Qt::Horizontal, container);
        manager = ManagerT(slider, min_value, max_value);

        field = new QHBoxLayout();
        field->addWidget(box, 25);
        field->addWidget(slider, 75);
        field->setSpacing(7);
        layout->addRow(label, field);

        param.value = qBound(min_value, param.value, max_value);
        UpdateValue(param.value);

        auto on_update = [=, this](T value){ param.value = value; UpdateValue(value); };
        CONNECT(box, &SpinBoxT::valueChanged, on_update);
        CONNECT(slider, &QSlider::valueChanged, [=, this](int value){ on_update(manager.ConvertFromRaw(value)); });
    }

    ~InspectorNumericField()
    {
        delete label;
        delete box;
        delete slider;
        delete field;
    }
private:
    void UpdateValue(T value)
    {
        box->setValue(value);
        slider->setValue(manager.ConvertToRaw(value));
    }
};

template <typename T>
using InspectorIntegerField = InspectorNumericField<T, QSpinBox, IntSliderManager>;

template <typename T>
using InspectorFractionalField = InspectorNumericField<T, QDoubleSpinBox, DoubleSliderManager>;

#endif // INSPECTORNUMERICFIELD_H
