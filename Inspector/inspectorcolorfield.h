#ifndef INSPECTORCOLORFIELD_H
#define INSPECTORCOLORFIELD_H

#include <QColorDialog>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include "inspectoritem.h"
#include "coloricon.h"
#include "lockableobject.h"
#include "Utils/icons.h"
#include "Utils/colorutility.h"

class InspectorColorField : public InspectorItem
{
private:
    QLabel *label;
    ColorIcon *icon;
    QLineEdit *input;
    QPushButton *dialog_button;
    QHBoxLayout *field;
    void ChangeColor(const QColor &color);
public:
    InspectorColorField(QWidget *container, QFormLayout *layout, const char *name, QColor &value, LockableObject *scene);
    ~InspectorColorField();
};

#endif // INSPECTORCOLORFIELD_H
