#ifndef INSPECTORCOLORFIELD_H
#define INSPECTORCOLORFIELD_H

#include <QColorDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QRegularExpressionValidator>
#include "inspectableparam.h"
#include "inspectoritem.h"
#include "coloricon.h"

class InspectorColorField : public InspectorItem
{
private:
    QLabel *label;
    ColorIcon *icon;
    QLineEdit *input;
    QHBoxLayout *field;
    void ChangeColor(const QColor &color);
public:
    InspectorColorField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<QColor> &param);
    ~InspectorColorField();
};

#endif // INSPECTORCOLORFIELD_H
