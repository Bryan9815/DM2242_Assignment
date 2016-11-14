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
    int count = 0;
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if (((*it)->GetName() == Name_Of_Entity_To_Find) && (*it)->GetPosition() != Pos_of_finder)
        {
            Vector3 t1((*it)->GetPosition());
            Vector3 t2(Pos_of_finder);
            Vector3 distsq = (t1 - t2);
            float distsqF = distsq.Length();
            if (temp > distsqF || count == 0)
            {
                temp = distsqF;
                count++;
            }                
        }
    }
    return temp;
}

float EntityManager::FindDistanceBetweenEntities(string Name_Of_Finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    int count = 0;
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if (((*it)->GetName() == Name_Of_Entity_To_Find))//entity to find
        {
            for (vector<BaseEntity*>::iterator it2 = EntityList.begin(); it2 != EntityList.end(); ++it2)
            {
                if ((*it2)->GetName() == Name_Of_Finder)//finder
                {
                    Vector3 t1((*it)->GetPosition());
                    Vector3 t2((*it2)->GetPosition());
                    Vector3 distsq = (t1 - t2);
                    float distsqF = distsq.Length();
                    if (temp > distsqF || count == 0)
                    {
                        temp = distsqF;
                        count++;
                    }
                }                
            }            
        }
    }
    return temp;
}

Vector3 EntityManager::FindNearestEntity_Pos(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    int count = 0;
    BaseEntity* tempEntity;
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)
        {
            float distsq = ((*it)->GetPosition() - Pos_of_finder).LengthSquared();
            if (temp > distsq || count == 0)
            {
                temp = distsq;
                tempEntity = (*it);
                count++;
            }
                
        }
    }
    if (count == 0)
        return NULL;
    return tempEntity->GetPosition();
}

Vector3 EntityManager::FindNearestEntity_Pos(string Name_Of_Finder, string Name_Of_Entity_To_Find)
{
    float temp(0.0f);
    int count = 0;
    BaseEntity* tempEntity;
    for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)//entity to find
        {
            for (vector<BaseEntity*>::iterator it2 = EntityList.begin(); it2 != EntityList.end(); ++it2)
            {
                if ((*it2)->GetName() == Name_Of_Finder)//finder
                {
                    float distsq = ((*it)->GetPosition() - (*it2)->GetPosition()).LengthSquared();
                    if (temp > distsq || count == 0)
                    {
                        temp = distsq;
                        tempEntity = (*it);
                        count++;
                    }
                }
            }
            

        }
    }
    if (count == 0)
        return NULL;
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

void EntityManager::IncreaseEntityAggro(string Name_Of_Entity_To_Find, unsigned int amount_to_increase)
{
	for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
    {
        if ((*it)->GetName() == Name_Of_Entity_To_Find)
        {
            (*it)->SetAggro((*it)->GetAggro() + amount_to_increase);
        }
    }
}

unsigned int EntityManager::FindEntityAggro(string Name_Of_Entity_To_Find)
{
	for (vector<BaseEntity*>::iterator it = EntityList.begin(); it != EntityList.end(); ++it)
	{
		if ((*it)->GetName() == Name_Of_Entity_To_Find)
		{
			return (*it)->GetAggro();
		}
	}
}