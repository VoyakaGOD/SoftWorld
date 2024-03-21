#ifndef LOCKABLEOBJECT_H
#define LOCKABLEOBJECT_H

class LockableObject
{
public:
    virtual void Lock() = 0;
    virtual void Unlock() = 0;
};

#endif // LOCKABLEOBJECT_H
