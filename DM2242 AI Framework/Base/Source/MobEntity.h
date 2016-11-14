#ifndef MOB_ENTITY_H
#define MOB_ENTITY_H
#include "BaseEntity.h"
#include "StateMachine.h"

class MobEntity : public BaseEntity
{
public:
    MobEntity();
    ~MobEntity();

    void Init();
    void Update();
    void Delete();

	void DetermineTarget();
private:
	bool WarriorTrigger, HealerTrigger, RangerTrigger, WarriorKill, HealerKill, RangerKill, WarriorSkill;
	std::string Target;

	unsigned int WarriorAggro, HealerAggro, RangerAggro;

	float DistFromTarget;

    StateMachine MobSM;
};

#endif
