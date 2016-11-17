#ifndef WARRIOR_H
#define WARRIOR_H

#include "EntityManager.h"

class Warrior : public BaseEntity
{
public:
	Warrior();
	~Warrior();

	void Init(EntityManager* EManager, float world_width, float world_height);
	void Init(EntityManager* EManager, float world_width, float world_height, Vector3 startpos);
	void Update(double dt);
	void Delete();

	void WrapAroundScreen();

	float WarriorMobDist;
	float RangerMobDist;
	float HealerMobDist;

	StateMachine WarriorSM;
private:
	bool DeadAlly;
	bool EnemyNearAlly;
	
	float Speed;
	float AttackRange;
	float Cooldown;
	float ReviveRange;

	EntityManager* EManager;
};

#endif