#include "inspectorlabel.h"

InspectorLabel::InspectorLabel(QWidget *container, QFormLayout *layout, const char *name, const QString &text)
{
    name_label = new QLabel(name, container);
    text_label = new QLabel(text, container);
    text_label->setWordWrap(true);
    text_label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    layout->addRow(name_label, text_label);
}

InspectorLabel::~InspectorLabel()
{
    delete name_label;
    delete text_label;
}

void InspectorLabel::SetSignal(InspectorLabelManager *sender)
{
    CONNECT(sender, &InspectorLabelManager::TextChanged, text_label, &QLabel::setText);
}
