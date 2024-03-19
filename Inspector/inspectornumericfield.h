#ifndef INSPECTORNUMERICFIELD_H
#define INSPECTORNUMERICFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
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
    InspectorNumericField(QWidget *container, QFormLayout *layout, const char *name, T &value, T min_value, T max_value)
    {
        label = new QLabel(name, container);

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

        value = qBound(min_value, value, max_value);
        UpdateValue(value);

        auto update = [&value, this](T new_value){ value = new_value; UpdateValue(new_value); };
        CONNECT(box, &SpinBoxT::valueChanged, update);
        CONNECT(slider, &QSlider::valueChanged, [update, this](int value){ update(manager.ConvertFromRaw(value)); });
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
        box->blockSignals(true);
        slider->blockSignals(true);
        box->setValue(value);
        slider->setValue(manager.ConvertToRaw(value));
        box->blockSignals(false);
        slider->blockSignals(false);
    }
};

template <typename T>
using InspectorIntegerField = InspectorNumericField<T, QSpinBox, IntSliderManager>;

template <typename T>
using InspectorFractionalField = InspectorNumericField<T, QDoubleSpinBox, DoubleSliderManager>;

#endif // INSPECTORNUMERICFIELD_H
