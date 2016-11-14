#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H
#include "BaseEntity.h"
#include <vector>
using namespace std;
class EntityManager
{
public:
    EntityManager();
    ~EntityManager();

    void Init();
    void Update();
    void AddEntity(BaseEntity* Entity_To_Add);
    void RemoveEntity(string Name_of_Entity_Removed);
    float FindDistanceBetweenEntities(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
    Vector3 FindNearestEntity_Pos(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
    void DecreaseEntityHP(string Name_Of_Entity_To_Find, int amount_to_decrease);
private:
    vector<BaseEntity*> EntityList;
};

#endif