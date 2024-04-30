#include "drawingstyle.h"

DrawingStyle::DrawingStyle(QColor main_color, QColor border_color, double border_thickness) :
    main_color(main_color), border_color(border_color), border_thickness(border_thickness) {}

void DrawingStyle::WidenInspectorContext()
{
    Inspector::AddHeader("drawing style", NORMAL_HEADER);
    Inspector::AddParam("main color", main_color);
    Inspector::AddParam("border color", border_color);
    Inspector::AddParam("border thickness", border_thickness, 0, 25);
}
