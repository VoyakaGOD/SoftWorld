#ifndef EDITINGMANAGER_H
#define EDITINGMANAGER_H

class EditingManager
{
public:
    virtual void OnEditingStarted() = 0;
    virtual void OnEditingEnded() = 0;
};

#endif // EDITINGMANAGER_H
