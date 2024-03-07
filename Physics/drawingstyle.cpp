#include "drawingstyle.h"

DrawingStyle::DrawingStyle(QColor main_color, QColor border_color, double border_thickness) :
    main_color(main_color), border_color(border_color), border_thickness(border_thickness) {}

void DrawingStyle::AddToInspectableParamsList(InspectableParamsList &list)
{
    list.params.push_back(InspectableColor("main color", main_color));
    list.params.push_back(InspectableColor("border color", border_color));
    list.params.push_back(InspectableDouble("border thickness", border_thickness));
}
