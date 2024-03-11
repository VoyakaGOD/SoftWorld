#include "drawingstyle.h"

DrawingStyle::DrawingStyle(QColor main_color, QColor border_color, double border_thickness) :
    main_color(main_color), border_color(border_color), border_thickness(border_thickness) {}

void DrawingStyle::WidenInspectorContext(InspectorContext &context)
{
    context.AddParam("main color", main_color);
    context.AddParam("border color", border_color);
    context.AddParam("border thickness", border_thickness);
}
