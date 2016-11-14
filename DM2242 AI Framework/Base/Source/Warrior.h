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
private:
	bool DeadAlly;
	bool EnemyNearAlly;
	bool Attack;
	
	float WarriorMobDist;
	float AllyMobDist;
	float Speed;
	float AttackRange;
	float Cooldown;

	StateMachine WarriorSM;
	EntityManager* EManager;
};

#endif