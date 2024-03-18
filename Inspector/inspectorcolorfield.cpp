#include "inspectorcolorfield.h"

static QColor GetRandomColor()
{
    return QColor(rand() % 255, rand() %255, rand() % 255);
}

InspectorColorField::InspectorColorField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<QColor> &param)
{
    label = new QLabel(param.name, container);

    icon = new ColorIcon(param.value, 20, container);

    input = new QLineEdit(param.value.name(), container);
    QString pattern = QString("#%1%1%1%1%1%1").arg("[0-9a-fA-F]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression(pattern));
    input->setValidator(validator);
    input->setContextMenuPolicy(Qt::PreventContextMenu);

    field = new QHBoxLayout(container);
    field->addWidget(icon);
    field->addWidget(input);
    field->setSpacing(7);

    layout->addRow(label, field);

    auto update = [=, this](const QColor &color){ param.value = color; ChangeColor(color); };
    CONNECT(input, &QLineEdit::textChanged, [=](const QString &text){ if(text.length() == 7) update(QColor(text)); });
    CONNECT(icon, &ColorIcon::clicked, [=](){ update(GetRandomColor()); });
}

InspectorColorField::~InspectorColorField()
{
    delete label;
    delete icon;
    delete input;
    delete field;
}

void InspectorColorField::ChangeColor(const QColor &color)
{
    icon->SetColor(color);
    input->setText(color.name());
}
