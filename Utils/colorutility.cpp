#include "colorutility.h"

static auto validator = new QRegularExpressionValidator(QRegularExpression(QString("^#[A-Fa-f0-9]{6}$")));

ColorUtility::ColorUtility() {}

QColor ColorUtility::GetRandomColor()
{
    return QColor(rand() % 255, rand() % 255, rand() % 255);
}

const QRegularExpressionValidator *ColorUtility::GetValidator()
{
    return validator;
}
