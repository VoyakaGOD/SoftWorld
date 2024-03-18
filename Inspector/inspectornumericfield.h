#ifndef INSPECTORNUMERICFIELD_H
#define INSPECTORNUMERICFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
#include "inspectableparam.h"
#include "inspectoritem.h"

template <typename T, typename SpinBoxT, typename ImplicitT>
class InspectorNumericField : public InspectorItem
{
private:
    QLabel *label;
    SpinBoxT *box;
    QSlider *slider;
    QHBoxLayout *field;
public:
    InspectorNumericField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<T> &param)
    {
        ImplicitT max_value = static_cast<ImplicitT>(param.max);
        ImplicitT min_value = static_cast<ImplicitT>(param.min);

        label = new QLabel(param.name, container);

        box = new SpinBoxT(container);
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

        auto on_update = [=, this](ImplicitT value){ param.value = static_cast<T>(value); UpdateValue(value); };
        CONNECT(box, &SpinBoxT::valueChanged, on_update);
        CONNECT(slider, &QSlider::valueChanged, on_update);

        param.value = qBound(min_value, param.value, max_value);
        UpdateValue(param.value);
    }

    ~InspectorNumericField()
    {
        delete label;
        delete box;
        delete slider;
        delete field;
    }
private:
    void UpdateValue(ImplicitT value)
    {
        box->setValue(value);
        slider->setValue(value);
    }
};

template <typename T>
using InspectorIntegerField = InspectorNumericField<T, QSpinBox, int>;

template <typename T>
using InspectorFractionalField = InspectorNumericField<T, QDoubleSpinBox, double>;

#endif // INSPECTORNUMERICFIELD_H
