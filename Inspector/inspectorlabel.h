#ifndef INSPECTORLABEL_H
#define INSPECTORLABEL_H

#include <QLabel>
#include <QFormLayout>
#include "inspectoritem.h"
#include "editingmanager.h"
#include "inspectorlabelmanager.h"

class InspectorLabel : public InspectorItem
{
private:
    QLabel *name_label;
    QLabel *text_label;
    QMetaObject::Connection connection;

public:
    InspectorLabel(QWidget *container, QFormLayout *layout, const char *name, const QString &text);
    ~InspectorLabel();
    void SetSignal(InspectorLabelManager *sender);
};

#endif // INSPECTORLABEL_H
