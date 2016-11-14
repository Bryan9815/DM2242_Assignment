#ifndef RANGER_ENTITY_H
#define RANGER_ENTITY_H
#include "EntityManager.h"
class RangerEntity : public BaseEntity
{
public:
    RangerEntity();
    ~RangerEntity();

    void Init(EntityManager* Entity_Manager);
    void Update();
    void Delete();
    void StateCheck();
    void StateRun();
    void UpdateVariables();
    void Revive();
    
private:
    bool DeadAlly;
    float NearestDeadAllyDist;
    float NearestEnemyDist;
    int NearEnemies;
    StateMachine *RangerSM;
    EntityManager* Entity_Manager;
    float AttackRange;
    float AttackReset_Timer;
};

#endif
