#ifndef SOFTSCENE_H
#define SOFTSCENE_H

#include <QMutex>
#include "physicalbody.h"
#include <Serialize/deser_helpers.h>
#include "Inspector/inspector.h"

///SERIALIZABLE SoftScene SAVED_OBJ_SCENE inplace

//time limit for try to lock
#define SOFT_SCENE_REQUEST_TIME_LIMIT 100 //milliseconds

class SoftScene: public SerializableObject
{
private:
    mutable QMutex synchronizer;
    list<PhysicalBody*> bodies;
    double air_density;
    double g;
    InspectorLabelManager count_label_manager;

public: // serialize
    virtual void SaveID(DataStorageWriter &data) const;
    virtual size_t GetSavedSize() const;
    virtual void SaveData(DataStorageWriter &data) const;
    static int Deserialize(SerializableObject*, DataStorageReader &);

public:
    QRect world_rect;

    SoftScene(const QRect &world_rect, double air_density, double g);
    ~SoftScene();

    void Draw(QPainter &painter) const;
    void DoNextStep(double delta_time);
    void AddBody(PhysicalBody *body);
    void RemoveBody(PhysicalBody *body);
    void Clear();
    void WidenInspectorContext();
    PhysicalBody *GetBodyAt(const QPoint &point) const;
    void Lock() const;
    bool TryToLock() const;
    void Unlock() const;
};

#endif // SOFTSCENE_H
