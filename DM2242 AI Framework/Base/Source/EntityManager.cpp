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
void EntityManager::Update(double dt)
{
    /*for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        (*it)->Update();
    }*/
    for (int i = 0; i < (int)EntityList.size(); ++i)
    {
        EntityList[i]->Update(dt);
    }
}

void EntityManager::AddEntity(BaseEntity* Entity_To_Add)
{
    EntityList.push_back(Entity_To_Add);
}
void EntityManager::RemoveEntity(string Name_of_Entity_Removed)
{
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_of_Entity_Removed)
        {
            EntityList.erase(it);
            return;
        }
    }
}

float EntityManager::FindDistanceBetweenEntities(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if (((*it)->GetName() == Name_Of_Entity_To_Find) && (*it)->GetPosition() != Pos_of_finder)
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

void EntityManager::DecreaseEntityHP(string Name_Of_Entity_To_Find, int amount_to_decrease)
{
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)
        {
            (*it)->SetHP((*it)->GetHP() - amount_to_decrease);
        }
    }
}