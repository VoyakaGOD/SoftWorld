#ifndef DRAWINGSTYLE_H
#define DRAWINGSTYLE_H

#include <QColor>
#include "Inspector/inspector.h"

class DrawingStyle
{
public:
    QColor main_color;
    QColor border_color;
    double border_thickness;
    DrawingStyle(QColor main_color = QColor(0, 255, 0, 128), QColor border_color = Qt::black, double border_thickness = 1);
    void WidenInspectorContext();
};

#endif // DRAWINGSTYLE_H
