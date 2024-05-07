#ifndef LINESINTERSECTIONINFO_H
#define LINESINTERSECTIONINFO_H

#include "polypoint.h"

struct LinesIntersectionInfo
{
    PolyPoint &first_line_start;
    PolyPoint &first_line_end;
    PolyPoint &second_line_start;
    PolyPoint &second_line_end;
    QVector2D intersection_point;
    LinesIntersectionInfo(PolyPoint &first_line_start, PolyPoint &first_line_end, PolyPoint &second_line_start,
        PolyPoint &second_line_end, QVector2D intersection_point);
};

#endif // LINESINTERSECTIONINFO_H
