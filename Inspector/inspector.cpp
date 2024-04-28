#include "inspector.h"

QWidget *Inspector::container;
QFormLayout *Inspector::layout;
vector<InspectorItem *> Inspector::items;
EditingManager *Inspector::manager;
void *Inspector::target;

Inspector::Inspector() {}

void Inspector::Mount(QWidget *container, QFormLayout *layout, EditingManager *default_manager)
{
    Inspector::container = container;
    Inspector::layout = layout;
    Inspector::manager = default_manager;
    Inspector::target = nullptr;
}

void Inspector::SetManager(EditingManager *manager)
{
    Inspector::manager = manager;
}

void Inspector::SetTarget(void *target, EditingManager *manager)
{
    if(manager)
        Inspector::manager = manager;
    Inspector::target = target;
    Clear();
}

bool Inspector::IsTarget(void *ptr)
{
    return target == ptr;
}

void *Inspector::GetTarget()
{
    return target;
}

void Inspector::Clear()
{
    for(int i = 0; i < items.size(); i++)
        delete items[i];
    items.clear();
}

void Inspector::AddHeader(const char *text, int size)
{
    items.push_back(new InspectorHeader(container, layout, text, size));
}

void Inspector::AddAction(const char *name, Action action)
{
    items.push_back(new InspectorButton(container, layout, name, action, manager));
}

void Inspector::AddLabel(const char *name, const QString &text, InspectorLabelManager *manager)
{
    InspectorLabel *label = new InspectorLabel(container, layout, name, text, manager ? &(manager->valid) : nullptr);
    items.push_back(label);

    if(manager)
    {
        manager->label = label;
        manager->valid = true;
    }
}

void Inspector::AddParam(const char *name, QColor &value)
{
    items.push_back(new InspectorColorField(container, layout, name, value, manager));
}

void Inspector::AddParam(const char *name, int &value, int min, int max)
{
    items.push_back(new InspectorIntegerField(container, layout, name, value, min, max, manager));
}

void Inspector::AddParam(const char *name, unsigned long &value, unsigned long min, unsigned long max)
{
    items.push_back(new InspectorIntegerField(container, layout, name, value, min, max, manager));
}

void Inspector::AddParam(const char *name, double &value, double min, double max)
{
    items.push_back(new InspectorFractionalField(container, layout, name, value, min, max, manager));
}

void Inspector::AddParam(const char *name, float &value, float min, float max)
{
    items.push_back(new InspectorFractionalField(container, layout, name, value, min, max, manager));
}

void Inspector::AddParam(const char *name, QString &value, int max_length)
{
    items.push_back(new InspectorStringField(container, layout, name, value, max_length, manager));
}
