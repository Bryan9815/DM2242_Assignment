#ifndef WARRIOR_H
#define WARRIOR_H

#include "EntityManager.h"

class Warrior : public BaseEntity
{
public:
	Warrior();
	~Warrior();

	void Init();
	void Update(double dt);
	void Delete();
private:
	bool DeadAlly;
	bool EnemyNearAlly;
	
	float WarriorMobDist;
	float AllyMobDist;

	StateMachine WarriorSM;
	EntityManager EManager;
};

#endif