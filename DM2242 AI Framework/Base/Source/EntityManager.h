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

private:
    vector<BaseEntity> EntityList;
};

#endif