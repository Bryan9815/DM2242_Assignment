#ifndef HEALER_ENTITY_H
#define HEALER_ENTITY_H
#include "EntityManager.h"

class HealerEntity : public BaseEntity
{
public:
    HealerEntity();
    ~HealerEntity();

    void Init(EntityManager* Entity_Manager, float world_width, float world_height);
    void Init(EntityManager* Entity_Manager, float world_width, float world_height, Vector3 startpos);
    void Update(double dt);
    void Delete();
    void StateCheck();
    void StateRun(double dt);
    void UpdateVariables(double dt);
    void Revive();

    void WrapAroundScreen();
    StateMachine HealerSM;
private:
    bool DeadAlly;
    float NearestDeadAllyDist;
    float NearestEnemyDist;
    int NearEnemies;
    EntityManager* Entity_Manager;
    float AttackRange;
    float AttackReset_Timer;
    int AttackDamage;
    float MovementSpeed;
    int healAmt;
};




#endif