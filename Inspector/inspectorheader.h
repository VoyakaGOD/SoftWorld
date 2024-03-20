#ifndef INSPECTORHEADER_H
#define INSPECTORHEADER_H

#include <QLabel>
#include <QFormLayout>
#include "inspectoritem.h"

#define SMALL_HEADER 5
#define NORMAL_HEADER 3
#define LARGE_HEADER 1

class InspectorHeader : public InspectorItem
{
private:
    QLabel *label;
public:
    InspectorHeader(QWidget *container, QFormLayout *layout, const char *text, int size);
    ~InspectorHeader();
};

#endif // INSPECTORHEADER_H
