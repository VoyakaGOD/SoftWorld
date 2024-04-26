#include "inspectorstringfield.h"

InspectorStringField::InspectorStringField(QWidget *container, QFormLayout *layout, const char* name,
    QString &value, int max_length, EditingManager *manager)
{
    label = new QLabel(name, container);

    input = new QLineEdit(value, container);
    input->setMaxLength(max_length);
    input->setContextMenuPolicy(Qt::PreventContextMenu);

    layout->addRow(label, input);

    CONNECT(input, &QLineEdit::textChanged, [&value, manager](const QString &text){
        if(!manager->PrepareForEditing())
            return;
        value = text;
        manager->EndEditing();
    });
}

InspectorStringField::~InspectorStringField()
{
    delete label;
    delete input;
}
