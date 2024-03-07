#ifndef DRAWINGSTYLE_H
#define DRAWINGSTYLE_H

#include <QColor>

class DrawingStyle
{
public:
    QColor main_color;
    QColor border_color;
    double border_thickness;
    DrawingStyle(QColor main_color, QColor border_color, double border_thickness);
};

#endif // DRAWINGSTYLE_H
