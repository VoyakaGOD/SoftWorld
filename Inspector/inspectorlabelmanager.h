#ifndef INSPECTORLABELMANAGER_H
#define INSPECTORLABELMANAGER_H

#include "inspectorlabel.h"

class InspectorLabelManager : public QObject
{
    Q_OBJECT

private:
    bool valid;
    InspectorLabel *label;

public:
    InspectorLabelManager();
    bool IsValid() const;
    void ChangeText(const QString &text);

signals:
    void TextChanged(const QString &text);

    friend class Inspector;
};

#endif // INSPECTORLABELMANAGER_H
