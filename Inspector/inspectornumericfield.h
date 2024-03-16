#ifndef INSPECTORNUMERICFIELD_H
#define INSPECTORNUMERICFIELD_H

#include <QLabel>
#include <QFormLayout>
#include "inspectableparam.h"
#include "inspectoritem.h"

class InspectorNumericField : public InspectorItem
{
private:
    QLabel *label;

public:
    InspectorNumericField(QWidget *container, QFormLayout *layout, const InspectableParam &param);
    ~InspectorNumericField();
};

#endif // INSPECTORNUMERICFIELD_H
