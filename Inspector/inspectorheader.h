#ifndef INSPECTORHEADER_H
#define INSPECTORHEADER_H

#include <QLabel>
#include <QFormLayout>
#include "inspectoritem.h"

class InspectorHeader : public InspectorItem
{
private:
    QLabel *label;
public:
    InspectorHeader(QWidget *container, QFormLayout *layout, const char *text, int priority);
    ~InspectorHeader();
};

#endif // INSPECTORHEADER_H
