#ifndef INSPECTORNUMERICFIELD_H
#define INSPECTORNUMERICFIELD_H

#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSlider>
#include "inspectoritem.h"
#include "slidermanagers.h"
#include "editingmanager.h"

template <typename T, typename SpinBoxT, typename ManagerT>
class InspectorNumericField : public InspectorItem
{
private:
    QLabel *label;
    SpinBoxT *box;
    QSlider *slider;
    QHBoxLayout *field;
    ManagerT box_manager;
public:
    InspectorNumericField(QWidget *container, QFormLayout *layout,
        const char *name, T &value, T min_value, T max_value, EditingManager *manager)
    {
        label = new QLabel(name, container);

        box = new SpinBoxT(container);
        box->setMaximum(max_value);
        box->setMinimum(min_value);
        box->setContextMenuPolicy(Qt::PreventContextMenu);

        slider = new QSlider(Qt::Horizontal, container);
        box_manager = ManagerT(slider, min_value, max_value);

        field = new QHBoxLayout();
        field->addWidget(box, 25);
        field->addWidget(slider, 75);
        field->setSpacing(7);
        layout->addRow(label, field);

        if(manager->PrepareForEditing())
        {
            value = qBound(min_value, value, max_value);
            manager->EndEditing();
            UpdateValue(value);
        }

        auto update = [&value, manager, this](T new_value){
            if(!manager->PrepareForEditing())
                return;
            value = new_value;
            manager->EndEditing();
            UpdateValue(new_value);
        };
        CONNECT(box, &SpinBoxT::valueChanged, update);
        CONNECT(slider, &QSlider::valueChanged, [update, this](int value){
            update(box_manager.ConvertFromRaw(value));
        });
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
        slider->setValue(box_manager.ConvertToRaw(value));
        box->blockSignals(false);
        slider->blockSignals(false);
    }
};

template <typename T>
using InspectorIntegerField = InspectorNumericField<T, QSpinBox, IntSliderManager>;

template <typename T>
using InspectorFractionalField = InspectorNumericField<T, QDoubleSpinBox, DoubleSliderManager>;

#endif // INSPECTORNUMERICFIELD_H
