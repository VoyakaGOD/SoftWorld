#include "softscene.h"

SoftScene::SoftScene(QRect world_rect, double air_density, double g):
    world_rect(world_rect), air_density(air_density), g(g) {}

void SoftScene::Draw(QPainter &painter) const
{
    throw "not implemented yet!";
}

void SoftScene::DoNextStep(double delta_time)
{
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
    bodies.push_back(body);
}

void SoftScene::RemoveBody(PhysicalBody *body)
{
    bodies.remove(body);
}

InspectableParamsList SoftScene::GetInspectableParams()
{
    InspectableParamsList list("scene", 2);
    list.params.push_back(InspectableDouble("air density", air_density));
    list.params.push_back(InspectableDouble("g", g));
    return list;
}

PhysicalBody *SoftScene::GetBodyAt(const QPoint &point) const
{
    for(auto body : bodies)
    {
        if(body->ContainsPoint(point))
            return body;
    }
    return nullptr;
}
