#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <vector>
#include <QFormLayout>
#include "inspectorheader.h"
#include "inspectorbutton.h"
#include "inspectorcolorfield.h"
#include "inspectornumericfield.h"

using namespace std;

class Inspector final
{
private:
    static QWidget *container;
    static QFormLayout *layout;
    static vector<InspectorItem *> items;
    static EditingManager *manager;
    static void *target;

public:
    static void Mount(QWidget *container, QFormLayout *layout, EditingManager *default_manager);
    static void SetTarget(void *target, EditingManager *manager);
    static bool IsTarget(void *ptr);
    static void Clear();
    static void AddHeader(const char *text, int size);
    static void AddAction(const char *name, Action action);
    static void AddParam(const char *name, QColor &value);
    static void AddParam(const char *name, int &value, int min = INT_MIN, int max = INT_MAX);
    static void AddParam(const char *name, double &value, double min = -1e35, double max = 1e35);
    static void AddParam(const char *name, float &value, float min = -1e305, float max = 1e305);

private:
    Inspector();
};

#endif // INSPECTOR_H
