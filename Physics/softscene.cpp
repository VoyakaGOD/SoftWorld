#include "softscene.h"
#include <Serialize/deserialize.h>
#include <Serialize/serialize.h>

struct SoftSceneData{
    obj_fixed_data_len_t size;
    int bodycnt;
    double air_density;
    double g;
    QRect world_rect;
};

static SoftSceneData default_data = {
    .bodycnt = 0,
    .air_density = 1,
    .g = 10,
    .world_rect = QRect(0,0,500,500)
};

SoftScene::SoftScene(const QRect &world_rect, double air_density, double g, double throwing_scale):
    world_rect(world_rect), air_density(air_density), g(g), throwing_scale(throwing_scale) {}

void SoftScene::RemoveAllBodies()
{
    QMutexLocker lock(&synchronizer);

    for (PhysicalBody* body: bodies)
    {
        delete body;
    }

    bodies.clear();
}

SoftScene::~SoftScene()
{
    RemoveAllBodies();
}

size_t SoftScene::GetSavedSize() const {
    size_t s = sizeof(saved_obj_id_t) + sizeof(SoftSceneData);
    for (auto i = this->bodies.begin(); i != this->bodies.end(); i++) {
        s += (*i)->GetSavedSize();
    }
    s += sizeof(saved_obj_id_t);
    return s;
}

void SoftScene::SaveData(DataStorageWriter &writer) const {

    SoftSceneData* sdata = (SoftSceneData*)(writer.data);
    WRITER_MOVE_BYTES(writer, sizeof(*sdata));

    sdata->size = sizeof(*sdata);
    sdata->bodycnt = this->bodies.size();
    sdata->air_density = this->air_density;
    sdata->g = this->g;
    sdata->world_rect = this->world_rect;

    for (auto i = this->bodies.begin(); i != this->bodies.end(); i++) {
        saveObj(writer, *(*i));
    }
    PTR_APPEND(writer.data, saved_obj_id_t, SAVED_OBJ_NONE)
}

int SoftScene::Deserialize(SerializableObject* obj, DataStorageReader & reader) {
    SoftScene* scene = dynamic_cast<SoftScene*>(obj);

    if (!scene) {
        return DESERR_INVTYPE;
    }

    READER_CHECK_FIXEDDATA_RET(reader, SoftSceneData)

    scene->g = GETDATA(reader, SoftSceneData, g);
    scene->air_density = GETDATA(reader, SoftSceneData, air_density);
    scene->world_rect = GETDATA(reader, SoftSceneData, world_rect);

    int bodycnt = GETDATA(reader, SoftSceneData, bodycnt);

    PTR_MOVE_BYTES(reader.data, ((SoftSceneData*)(reader.data))->size);

    scene->Clear();

    for (int i = 0; i < bodycnt; i++) {
        PhysicalBody* body = physicalBodyDeserialize(reader);
        if (!body)
            return last_deserialize_error;
        scene->AddBody(body);
    }

    return DESERR_OK;
}

void SoftScene::Draw(QPainter &painter) const
{
    if(!TryToLock())
        return;

    for(auto body : bodies)
    {
        body->Draw(painter);
    }

    synchronizer.unlock();
}

void SoftScene::DoNextStep(double delta_time)
{
    QMutexLocker lock(&synchronizer);

    for(auto body1 : bodies)
    {
        body1->ApplyGravity(air_density, g, delta_time);
        for(auto body2 : bodies)
        {
            if(body1 == body2)
                continue;
            if(body1->GetBoundingRect().intersects(body2->GetBoundingRect()))
                body1->SolveCollision(body2);
        }
    }

    for(auto body : bodies)
    {
        body->ApplyInternalRestrictions(delta_time);
        body->KeepSceneBorders(world_rect);
    }
}

void SoftScene::AddBody(PhysicalBody *body)
{
    QMutexLocker lock(&synchronizer);

    bodies.push_back(body);
    count_label_manager.ChangeText(QString::number(bodies.size()));
}

void SoftScene::RemoveBody(PhysicalBody *body)
{
    QMutexLocker lock(&synchronizer);

    bodies.remove(body);
    delete body;

    count_label_manager.ChangeText(QString::number(bodies.size()));
}

void SoftScene::Clear()
{
    RemoveAllBodies();

    count_label_manager.ChangeText("0");
}

void SoftScene::WidenInspectorContext()
{
    Inspector::AddHeader("scene", LARGE_HEADER);
    Inspector::AddParam("air density", air_density, (double)1, (double)1000);
    Inspector::AddParam("g", g, (double)0, (double)100);
    Inspector::AddParam("throwing scale", throwing_scale, (double)0.1, (double)1);
    Inspector::AddLabel("bodies count:", QString::number(bodies.size()), &count_label_manager);
}

PhysicalBody *SoftScene::GetBodyAt(const QPoint &point) const
{
    QMutexLocker lock(&synchronizer);

    for(auto body : bodies)
    {
        if(body->ContainsPoint(point))
            return body;
    }

    return nullptr;
}

void SoftScene::Lock() const
{
    synchronizer.lock();
}

bool SoftScene::TryToLock() const
{
    return synchronizer.tryLock(SOFT_SCENE_REQUEST_TIME_LIMIT);
}

void SoftScene::Unlock() const
{
    synchronizer.unlock();
}

double SoftScene::GetThrowingScale()
{
    return throwing_scale;
}
