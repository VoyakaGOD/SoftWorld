#include "inspectorcolorfield.h"

static QIcon CreateColorIcon(const QColor &color, int size)
{
    QPixmap map(size, size);
    map.fill(color);
    /*QPen pen(Qt::black, 1);
    QPainter painter(&map);
    painter.setPen(pen);
    painter.drawRect(QRect(0,0,size-1,size-1));*/
    return QIcon(map);
}

InspectorColorField::InspectorColorField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<QColor> &param)
{
    label = new QLabel(param.name, container);

    frame = new QFrame(container);
    frame->setFrameShape(QFrame::Box);
    QSize frame_size = QSize(frame->height(), frame->height());
    frame->setMinimumSize(frame_size);
    frame->setMaximumSize(frame_size);
    ChangeColor(param.value);

    input = new QLineEdit(param.value.name(), container);
    QString pattern = QString("#%1%1%1%1%1%1").arg("[0-9a-fA-F]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression(pattern));
    input->setValidator(validator);
    input->setContextMenuPolicy(Qt::PreventContextMenu);

    field = new QHBoxLayout(container);
    field->addWidget(frame);
    field->addWidget(input);
    field->setSpacing(7);

    layout->addRow(label, field);
    CONNECT(input, &QLineEdit::textChanged, [=](const QString &text){ param.value = QColor(text); ChangeColor(param.value); });
}

InspectorColorField::~InspectorColorField()
{
    delete label;
    delete frame;
    delete input;
    delete field;
}

void InspectorColorField::ChangeColor(QColor &color)
{
    frame->setStyleSheet(QString("background-color: %1;").arg(color.name()));
}
