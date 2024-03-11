#ifndef INSPECTABLEACTION_H
#define INSPECTABLEACTION_H

typedef void (*Action)();

class InspectableAction
{
public:
    const char *name;
    Action action;
    InspectableAction(const char *name, Action action);
};

#endif // INSPECTABLEACTION_H
