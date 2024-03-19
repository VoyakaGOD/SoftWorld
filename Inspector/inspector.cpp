#include "inspector.h"

QWidget *Inspector::container;
QFormLayout *Inspector::layout;
vector<InspectorItem *> Inspector::items;

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
    items.push_back(new InspectorButton(container, layout, name, action));
}

void Inspector::AddParam(const char *name, QColor &value)
{
    items.push_back(new InspectorColorField(container, layout, CertainInspectableParam(name, InspectableParamType::Int, value)));
}

void Inspector::AddParam(const char *name, int &value, int min, int max) {}
void Inspector::AddParam(const char *name, double &value, double min, double max) {}
void Inspector::AddParam(const char *name, float &value, float min, float max) {}
