#ifndef INSPECTORCOLORFIELD_H
#define INSPECTORCOLORFIELD_H

#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include "inspectableparam.h"
#include "inspectoritem.h"

class InspectorColorField : public InspectorItem
{
private:
    QLabel *label;
    QFrame *frame;
    QLineEdit *input;
    QHBoxLayout *field;
    void ChangeColor(QColor &color);
public:
    InspectorColorField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<QColor> &param);
    ~InspectorColorField();
};

#endif // INSPECTORCOLORFIELD_H
