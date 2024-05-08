#include "inspectorlabelmanager.h"

InspectorLabelManager::InspectorLabelManager() {}

void InspectorLabelManager::ChangeText(const QString &text)
{
    emit TextChanged(text);
}
