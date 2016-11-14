#ifndef WARRIOR_H
#define WARRIOR_H

#include "EntityManager.h"

class Warrior : public BaseEntity
{
public:
	Warrior();
	~Warrior();

    void Init(EntityManager* EManager);
    void Init(EntityManager* EManager, Vector3 startpos);
	void Update(double dt);
	void Delete();

	float WarriorMobDist;
	float RangerMobDist;
	float HealerMobDist;

	StateMachine WarriorSM;
private:
	bool DeadAlly;
	bool EnemyNearAlly;
	bool Attack;
	
	float Speed;
	float AttackRange;
	float Cooldown;

	EntityManager* EManager;
};

#endif