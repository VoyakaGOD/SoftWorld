#include "inspector.h"

QWidget *Inspector::container;
QFormLayout *Inspector::layout;
std::vector<InspectorItem *> Inspector::items;

Inspector::Inspector() {}

void Inspector::Mount(QWidget *container, QFormLayout *layout)
{
    Inspector::container = container;
    Inspector::layout = layout;
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
    items.push_back(new InspectorButton(container, layout, InspectableAction(name, action)));
}
