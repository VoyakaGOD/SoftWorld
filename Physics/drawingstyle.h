#ifndef DRAWINGSTYLE_H
#define DRAWINGSTYLE_H

#include <QColor>
#include "Inspector/inspectorcontext.h"

class DrawingStyle
{
public:
    QColor main_color;
    QColor border_color;
    double border_thickness;
    DrawingStyle(QColor main_color, QColor border_color, double border_thickness);
    void WidenInspectorContext(InspectorContext &context);
};

#endif // DRAWINGSTYLE_H
