#ifndef ICONS_H
#define ICONS_H

#include <QIcon>
#include <map>
#include "cstringcomparator.h"

class Icons final
{
private:
    static map<const char *, QIcon, CStringComparator> content;
public:
    static void AddIcon(const char *name, const char *path);
    static const QIcon &Get(const char *name);
private:
    Icons();
};

#endif // ICONS_H
