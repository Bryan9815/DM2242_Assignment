#include "EntityManager.h"


EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

void EntityManager::Init()
{

}
void EntityManager::Update()
{

}

void EntityManager::AddEntity()
{

}
void EntityManager::RemoveEntity()
{

}

float EntityManager::FindNearestEntity_Dist(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)
        {
            float distsq = ((*it)->GetPosition() - Pos_of_finder).LengthSquared();
            if (temp > distsq)
                temp = distsq;
        }
    }
    return temp;
}

Vector3 EntityManager::FindNearestEntity_Pos(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    BaseEntity* tempEntity;
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)
        {
            float distsq = ((*it)->GetPosition() - Pos_of_finder).LengthSquared();
            if (temp > distsq)
            {
                temp = distsq;
                tempEntity = (*it);
            }
                
        }
    }
    return tempEntity->GetPosition();
}