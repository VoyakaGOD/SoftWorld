#include "icons.h"

map<const char *, QIcon, CStringComparator> Icons::content;

Icons::Icons() {}

void Icons::AddIcon(const char *name, const char *path)
{
    QIcon new_icon;
    new_icon.addFile(QString::fromUtf8(path), QSize(), QIcon::Normal, QIcon::Off);
    content[name] = new_icon;
}

const QIcon &Icons::Get(const char *name)
{
    return content[name];
}
