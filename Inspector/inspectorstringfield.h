#ifndef INSPECTORSTRINGFIELD_H
#define INSPECTORSTRINGFIELD_H

#include <QLabel>
#include <QFormLayout>
#include <QLineEdit>
#include "inspectoritem.h"
#include "editingmanager.h"

class InspectorStringField : public InspectorItem
{
private:
    QLabel *label;
    QLineEdit *input;
public:
    InspectorStringField(QWidget *container, QFormLayout *layout, const char* name, QString &value, int max_length, EditingManager *manager);
    ~InspectorStringField();
};

#endif // INSPECTORSTRINGFIELD_H
