#include "inspectorlabelmanager.h"

InspectorLabelManager::InspectorLabelManager()
{
    valid = false;
    label = nullptr;
}

InspectorLabelManager::~InspectorLabelManager()
{
    //disconnect(this, nullptr, nullptr, nullptr);
}

bool InspectorLabelManager::IsValid() const
{
    return valid;
}

void InspectorLabelManager::ChangeText(const QString &text)
{
    if(!valid)
        return;

    emit TextChanged(text);
}
