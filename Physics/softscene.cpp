#include "softscene.h"

SoftScene::SoftScene(const QRect &world_rect, double air_density, double g):
    world_rect(world_rect), air_density(air_density), g(g) {}

void SoftScene::Draw(QPainter &painter) const
{
    lock_guard<mutex> lock(synchronizer);

    for(auto body : bodies)
    {
        body->Draw(painter);
    }
}

void SoftScene::DoNextStep(double delta_time)
{
    lock_guard<mutex> lock(synchronizer);

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
    lock_guard<mutex> lock(synchronizer);

    bodies.push_back(body);
}

void SoftScene::RemoveBody(PhysicalBody *body)
{
    lock_guard<mutex> lock(synchronizer);

    bodies.remove(body);
}

void SoftScene::WidenInspectorContext(InspectorContext &context)
{
    lock_guard<mutex> lock(synchronizer);

    context.SetTargetName("scene");
    context.AddParam("air density", air_density, (double)0, (double)1000);
    context.AddParam("g", g, (double)0, (double)100);
}

PhysicalBody *SoftScene::GetBodyAt(const QPoint &point) const
{
    lock_guard<mutex> lock(synchronizer);

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

void SoftScene::Unlock()
{
    synchronizer.unlock();
}
