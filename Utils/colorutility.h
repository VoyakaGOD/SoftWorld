#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QColor>
#include <QRegularExpressionValidator>

class ColorUtility final
{
public:
    static QColor GetRandomColor();
    static const QRegularExpressionValidator *GetValidator();
private:
    ColorUtility();
};

#endif // COLORUTILITY_H
