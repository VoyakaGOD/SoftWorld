#include "inspectorlabelmanager.h"

InspectorLabelManager::InspectorLabelManager()
{
    valid = false;
    label = nullptr;
}

bool InspectorLabelManager::IsValid() const
{
    return valid;
}

void InspectorLabelManager::ChangeText(const QString &text) const
{
    if(!valid)
        return;

    label->ChangeText(text);
}