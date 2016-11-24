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
    void Move(double dt);
    void PreventOverlap();

    void WrapAroundScreen();
    StateMachine HealerSM;
private:
    EntityManager* Entity_Manager;
    bool DeadAlly;
    float NearestDeadAllyDist;
    float NearestEnemyDist;
    int NearEnemies;    
    float AttackRange;
    float AttackReset_Timer;
    int AttackDamage;
    float MovementSpeed;
    int healAmt;
    float HealRange;
    float HealReset_Timer;
    bool InjuredAlly;
    BaseEntity* InjuredAllyEntity;
    
};




#endif