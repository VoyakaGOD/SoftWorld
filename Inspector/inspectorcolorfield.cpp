#include "inspectorcolorfield.h"

static QColor GetRandomColor()
{
    return QColor(rand() % 255, rand() % 255, rand() % 255);
}

InspectorColorField::InspectorColorField(QWidget *container, QFormLayout *layout, const char *name, QColor &value, LockableObject *scene)
{
    label = new QLabel(name, container);

    icon = new ColorIcon(value, 20, container);

    input = new QLineEdit(value.name(), container);
    QString pattern = QString("#%1%1%1%1%1%1").arg("[0-9a-fA-F]");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(QRegularExpression(pattern));
    input->setValidator(validator);
    input->setContextMenuPolicy(Qt::PreventContextMenu);

    QIcon dialog_icon;
    dialog_icon.addFile(QString::fromUtf8(":/Icons/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
    dialog_button = new QPushButton(dialog_icon, "", container);

    field = new QHBoxLayout(container);
    field->addWidget(icon);
    field->addWidget(input);
    field->addWidget(dialog_button);
    field->setSpacing(7);

    layout->addRow(label, field);

    auto update = [&value, this](const QColor &color){value = color;  ChangeColor(color); };
    CONNECT(input, &QLineEdit::textChanged, [=](const QString &text){ if(text.length() == 7) update(QColor(text)); });
    CONNECT(icon, &ColorIcon::clicked, [=](){ update(GetRandomColor()); });
    CONNECT(dialog_button, &QPushButton::clicked, [=, &value](){
        QColor color = QColorDialog::getColor(value, container);
        if(color.isValid())
            update(color);
    });
}

InspectorColorField::~InspectorColorField()
{
    delete label;
    delete icon;
    delete input;
    delete dialog_button;
    delete field;
}

void InspectorColorField::ChangeColor(const QColor &color)
{
    icon->SetColor(color);
    input->setText(color.name());
}
