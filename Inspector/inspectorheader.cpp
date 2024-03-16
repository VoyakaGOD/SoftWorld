#include "inspectorheader.h"

InspectorHeader::InspectorHeader(QWidget *container, QFormLayout *layout, const char *text, int priority)
{
    label = new QLabel(QString("<h%1>%2</h%1>").arg(QString::number(priority), text), container);
    layout->addRow(label);
    label->setAlignment(Qt::AlignHCenter);
}

InspectorHeader::~InspectorHeader()
{
    delete label;
}
