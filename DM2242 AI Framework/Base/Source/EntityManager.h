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
    void AddEntity();
    void RemoveEntity();
    float FindNearestEntity_Dist(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
    Vector3 FindNearestEntity_Pos(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
private:
    vector<BaseEntity*> EntityList;
};

#endif