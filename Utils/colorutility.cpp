#include "colorutility.h"

static auto validator = new QRegularExpressionValidator(QRegularExpression(QString("#%1%1%1%1%1%1").arg("[0-9a-fA-F]")));

ColorUtility::ColorUtility() {}

QColor ColorUtility::GetRandomColor()
{
    return QColor(rand() % 255, rand() % 255, rand() % 255);
}

const QRegularExpressionValidator *ColorUtility::GetValidator()
{
    return validator;
}
