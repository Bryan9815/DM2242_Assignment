#ifndef MOB_ENTITY_H
#define MOB_ENTITY_H

#include "EntityManager.h"

class MobEntity : public BaseEntity
{
public:
    MobEntity();
    ~MobEntity();

    void Init(EntityManager* EManager);
    void Init(EntityManager* EManager, Vector3 startpos);
    void Update(double dt);
    void Delete();

	void DetermineTarget();

	bool Stunned;
	StateMachine MobSM;
private:
	std::string Target;

	float DistFromTarget;
	float Speed;
	float AttackRange;
	float Cooldown;
	float StunTimer;
    float knockTimer;
    
	EntityManager* EManager;
};

#endif
