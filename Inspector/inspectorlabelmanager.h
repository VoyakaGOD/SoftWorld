#ifndef INSPECTORLABELMANAGER_H
#define INSPECTORLABELMANAGER_H

#include "inspectorlabel.h"

class InspectorLabelManager
{
private:
    bool valid;
    InspectorLabel *label;

public:
    InspectorLabelManager();
    bool IsValid() const;
    void ChangeText(const QString &text) const;

    friend class Inspector;
};

#endif // INSPECTORLABELMANAGER_H
