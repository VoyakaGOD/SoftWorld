#include "inspectorcolorfield.h"

InspectorColorField::InspectorColorField(QWidget *container, QFormLayout *layout, const CertainInspectableParam<QColor> &param)
{
    label = new QLabel(param.name, container);

    icon = new ColorIcon(param.value, 30, container);

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

    auto on_update = [=, this](const QColor &color){ param.value = color; ChangeColor(color); };
    CONNECT(input, &QLineEdit::textChanged, [=](const QString &text){ on_update(QColor(text)); });
    CONNECT(icon, &ColorIcon::clicked, [=](){ on_update(param.value == Qt::white ? Qt::black : Qt::white); });
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
}
