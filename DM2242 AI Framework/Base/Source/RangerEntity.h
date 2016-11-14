#ifndef RANGER_ENTITY_H
#define RANGER_ENTITY_H
#include "EntityManager.h"
class RangerEntity : public BaseEntity
{
public:
    RangerEntity();
    ~RangerEntity();

    void Init(EntityManager* Entity_Manager);
    void Init(EntityManager* Entity_Manager, Vector3 startpos);
    void Update(double dt);
    void Delete();
    void StateCheck();
    void StateRun(double dt);
    void UpdateVariables(double dt);
    void Revive();
    StateMachine RangerSM;
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
};

#endif
