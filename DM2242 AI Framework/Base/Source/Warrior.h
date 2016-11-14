#ifndef WARRIOR_H
#define WARRIOR_H

#include "BaseEntity.h"
#include "StateMachine.h"

class Warrior : public BaseEntity
{
public:
	Warrior();
	~Warrior();

	void Init();
	void Update();
	void Delete();
private:
	bool DeadAlly;
	bool EnemyNearAlly;
	
	float WarriorMobDist;
	float AllyMobDist;

	StateMachine WarriorSM;
};

#endif