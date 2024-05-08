#ifndef INSPECTORLABELMANAGER_H
#define INSPECTORLABELMANAGER_H

#include <QObject>
#include <QLabel>

class InspectorLabelManager : public QObject
{
    Q_OBJECT

public:
    InspectorLabelManager();
    void ChangeText(const QString &text);

signals:
    void TextChanged(const QString &text);
};

#endif // INSPECTORLABELMANAGER_H
