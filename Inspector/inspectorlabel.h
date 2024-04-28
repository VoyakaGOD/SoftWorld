#ifndef INSPECTORLABEL_H
#define INSPECTORLABEL_H

#include <QLabel>
#include <QFormLayout>
#include "inspectoritem.h"
#include "editingmanager.h"

class InspectorLabel : public InspectorItem
{
private:
    QLabel *name_label;
    QLabel *text_label;
    bool *manager_valid_ptr;

public:
    InspectorLabel(QWidget *container, QFormLayout *layout, const char *name, const QString &text, bool *manager_valid_ptr);
    ~InspectorLabel();
    void ChangeText(const QString &text);
};

#endif // INSPECTORLABEL_H
