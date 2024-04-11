#include "softscene.h"

SoftScene::SoftScene(const QRect &world_rect, double air_density, double g):
    world_rect(world_rect), air_density(air_density), g(g) {}

SoftScene::~SoftScene()
{
    for (PhysicalBody* body: bodies) {
        delete body;
    }
}

void SoftScene::PrepareToDraw(bool lock)
{
    if(prepared_to_draw)
        return;
    if(lock)
        synchronizer.lock();
    prepared_to_draw = true;
}

void SoftScene::Draw(QPainter &painter) const
{
    if(!prepared_to_draw)
        throw "try do draw scene that is not prepared";

    for(auto body : bodies)
    {
        body->Draw(painter);
    }

    prepared_to_draw = false;
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
}

void SoftScene::RemoveBody(PhysicalBody *body)
{
    QMutexLocker lock(&synchronizer);

    bodies.remove(body);
}

void SoftScene::WidenInspectorContext()
{
    //QMutexLocker lock(&synchronizer);

    Inspector::AddHeader("scene", LARGE_HEADER);
    Inspector::AddParam("air density", air_density, (double)0, (double)1000);
    Inspector::AddParam("g", g, (double)0, (double)100);
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

void SoftScene::Lock()
{
    synchronizer.lock();
}

bool SoftScene::TryToLock()
{
    return synchronizer.tryLock(SOFT_SCENE_REQUEST_TIME_LIMIT);
}

void SoftScene::Unlock()
{
    synchronizer.unlock();
}
