#ifndef MOB_ENTITY_H
#define MOB_ENTITY_H

#include "EntityManager.h"

class MobEntity : public BaseEntity
{
public:
    MobEntity();
    ~MobEntity();

    void Init();
    void Update(double dt);
    void Delete();

	void DetermineTarget();
private:
	bool WarriorTrigger, HealerTrigger, RangerTrigger, WarriorKill, HealerKill, RangerKill, WarriorSkill;
	bool Attack;
	std::string Target;

	unsigned int WarriorAggro, HealerAggro, RangerAggro;

	float DistFromTarget;
	float Speed;
	float AttackRange;
	float Cooldown;

    StateMachine MobSM;
	EntityManager EManager;
};

#endif
