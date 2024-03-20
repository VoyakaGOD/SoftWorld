#include "coloricon.h"

void ColorIcon::SetColor(const QColor &color)
{
    QPixmap map(size, size);
    map.fill(color);
    QPainter painter(&map);
    painter.setPen(QPen(QColor(255 - color.red(), 255 - color.green(), 255 - color.blue()), 1));
    painter.drawRect(QRect(0, 0, size-1, size-1));
    setPixmap(map);
}

ColorIcon::ColorIcon(const QColor &color, int size, QWidget* parent) : QLabel(parent)
{
    this->size = size;
    SetColor(color);
}

void ColorIcon::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
