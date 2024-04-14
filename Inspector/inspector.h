#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <vector>
#include <QFormLayout>
#include "inspectorheader.h"
#include "inspectorbutton.h"
#include "inspectorcolorfield.h"
#include "inspectornumericfield.h"
#include "inspectorstringfield.h"

using namespace std;

/*
 * About SetTarget, IsTarget and GetTarget methods:
 * Target is simply an empty pointer to the last object sent for inspection;
 * it is not used in any way by the inspector itself. At the same time, you can send several
 * objects with their own managers for inspection, but before each adding of parameters, do not forget
 * to call SetTarget, this will replace the manager for the following parameters.
 */

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
    static void AddParam(const char *name, QString &value, int max_length = 32767);

private:
    Inspector();
};

#endif // INSPECTOR_H
