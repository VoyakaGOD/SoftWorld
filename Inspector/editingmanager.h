#ifndef EDITINGMANAGER_H
#define EDITINGMANAGER_H

class EditingManager
{
public:
    virtual bool PrepareForEditing() = 0;
    virtual void EndEditing() = 0;
};

#endif // EDITINGMANAGER_H
