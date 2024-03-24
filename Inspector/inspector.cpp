#include "inspector.h"

QWidget *Inspector::container;
QFormLayout *Inspector::layout;
vector<InspectorItem *> Inspector::items;
LockableObject *Inspector::scene;

Inspector::Inspector() {}

void Inspector::Mount(QWidget *container, QFormLayout *layout, LockableObject *scene)
{
    Inspector::container = container;
    Inspector::layout = layout;
    Inspector::scene = scene;
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
    items.push_back(new InspectorButton(container, layout, name, action, scene));
}

void Inspector::AddParam(const char *name, QColor &value)
{
    items.push_back(new InspectorColorField(container, layout, name, value, scene));
}

void Inspector::AddParam(const char *name, int &value, int min, int max)
{
    items.push_back(new InspectorIntegerField(container, layout, name, value, min, max, scene));
}

void Inspector::AddParam(const char *name, double &value, double min, double max)
{
    items.push_back(new InspectorFractionalField(container, layout, name, value, min, max, scene));
}

void Inspector::AddParam(const char *name, float &value, float min, float max)
{
    items.push_back(new InspectorFractionalField(container, layout, name, value, min, max, scene));
}
