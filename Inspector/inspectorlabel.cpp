#include "inspectorlabel.h"

InspectorLabel::InspectorLabel(QWidget *container, QFormLayout *layout, const char *name, const QString &text, bool *manager_valid_ptr)
{
    name_label = new QLabel(name, container);
    text_label = new QLabel(text, container);
    text_label->setWordWrap(true);
    text_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    layout->addRow(name_label, text_label);

    this->manager_valid_ptr = manager_valid_ptr;
}

InspectorLabel::~InspectorLabel()
{
    if(manager_valid_ptr)
        *manager_valid_ptr = false;

    delete name_label;
    delete text_label;
}

void InspectorLabel::ChangeText(const QString &text)
{
    text_label->setText(text);
}
