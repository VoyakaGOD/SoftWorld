#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <vector>
#include <QFormLayout>
#include "inspectorheader.h"
#include "inspectorbutton.h"
#include "inspectorcolorfield.h"
#include "inspectornumericfield.h"
#include "inspectorstringfield.h"
#include "inspectorlabel.h"

using namespace std;

/*
 * First of all you should call Mount method to set up Inspector.
 * Remember that editing manager should always be correct(not null), except when Inspector have no fields.
 * This class provides UI to change simulation values.
 * To do it you should call one of AddParam methods.
 * Aslo you can attach actions by calling AddAction method.
 * You can create text labels by calling AddLabel method, which return controller of new label.
 * You can store pointer to current target by calling SetTarget method, which change target and manager and also clear Inspector.
 * Also you can call Clear method by yourself.
 * And each Inspector's field has his own manager. You can change current manager by calling SetManager.
 * And then each new field will have this new manager, but old fields will save old.
 * Remark: passing nullptr as managet into SetTarger saves old manager.
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
    static void SetManager(EditingManager *manager);
    static void SetTarget(void *target, EditingManager *manager = nullptr);
    static bool IsTarget(void *ptr);
    static void *GetTarget();
    static void Clear();
    static void AddHeader(const char *text, int size);
    static void AddAction(const char *name, Action action);
    static void AddLabel(const char *name, const QString &text, InspectorLabelManager *manager = nullptr);
    static void AddParam(const char *name, QColor &value);
    static void AddParam(const char *name, int &value, int min = INT_MIN, int max = INT_MAX);
    static void AddParam(const char *name, unsigned long &value, unsigned long min = 0, unsigned long max = 4294967295);
    static void AddParam(const char *name, double &value, double min = -1e35, double max = 1e35);
    static void AddParam(const char *name, float &value, float min = -1e305, float max = 1e305);
    static void AddParam(const char *name, QString &value, int max_length = 32767);

private:
    Inspector();
};

#endif // INSPECTOR_H
