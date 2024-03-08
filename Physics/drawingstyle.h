#ifndef DRAWINGSTYLE_H
#define DRAWINGSTYLE_H

#include <QColor>
#include "../Inspector/inspectableparamslist.h"

class DrawingStyle
{
public:
    QColor main_color;
    QColor border_color;
    double border_thickness;
    DrawingStyle(QColor main_color, QColor border_color, double border_thickness);
    void AddToInspectableParamsList(InspectableParamsList &list);
};

#endif // DRAWINGSTYLE_H
