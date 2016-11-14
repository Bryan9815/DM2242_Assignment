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
    void Update(double dt);
    void AddEntity(BaseEntity* Entity_To_Add);
    void RemoveEntity(string Name_of_Entity_Removed);
    float FindDistanceBetweenEntities(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
    Vector3 FindNearestEntity_Pos(Vector3 Pos_of_finder, string Name_Of_Entity_To_Find);
    void DecreaseEntityHP(string Name_Of_Entity_To_Find, int amount_to_decrease);
	void IncreaseEntityAggro(string Name_Of_Entity_To_Find, unsigned int amount_to_increase);
	//void MoveEntity(string Name_Of_Entity_To_Find, float distance);
    vector<BaseEntity*> EntityList;
private:
    
};

#endif