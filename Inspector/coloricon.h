#ifndef COLORICON_H
#define COLORICON_H

#include <QLabel>
#include <QPainter>

class ColorIcon : public QLabel
{
    Q_OBJECT
private:
    int size;
public:
    explicit ColorIcon(const QColor &color, int size, QWidget* parent = nullptr);
    void SetColor(const QColor &color);
signals:
    void clicked();
protected:
    void mousePressEvent(QMouseEvent* event);
};

#endif // COLORICON_H
