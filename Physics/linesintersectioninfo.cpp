#include "linesintersectioninfo.h"

LinesIntersectionInfo::LinesIntersectionInfo(PolyPoint &first_line_start, PolyPoint &first_line_end, PolyPoint &second_line_start,
    PolyPoint &second_line_end, QVector2D intersection_point) : first_line_start(first_line_start),first_line_end(first_line_end),
    second_line_start(second_line_start), second_line_end(second_line_end), intersection_point(intersection_point) {}
