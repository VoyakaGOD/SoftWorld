#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <vector>
#include <QFormLayout>
#include "inspectorheader.h"
#include "inspectorbutton.h"

class Inspector final
{
private:
    static QWidget *container;
    static QFormLayout *layout;
    static std::vector<InspectorItem *> items;

public:
    static void Mount(QWidget *container, QFormLayout *layout);
    static void Clear();
    static void AddHeader(const char *text, int size);
    static void AddAction(const char *name, Action action);

private:
    Inspector();
};

#endif // INSPECTOR_H
