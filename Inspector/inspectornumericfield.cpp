#include "inspectornumericfield.h"

InspectorNumericField::InspectorNumericField(QWidget *container, QFormLayout *layout, const InspectableParam &param)
{
    layout->addRow(param.name, (QWidget *)nullptr);

}

InspectorNumericField::~InspectorNumericField()
{

}
