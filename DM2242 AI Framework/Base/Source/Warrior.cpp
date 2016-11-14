#include "Warrior.h"

Warrior::Warrior()
{

}

Warrior::~Warrior()
{
	Delete();
}

void Warrior::Init()
{
	HP = 100;
	SetPosition(Vector3(0, 0, 0));
	Dead = false;
	DeadAlly = false;
	WarriorSM.AddState("Chase Enemy");
	WarriorSM.AddState("Attack");
	WarriorSM.AddState("Knockback");
	WarriorSM.AddState("Revive");
	WarriorSM.AddState("Dead");

	WarriorSM.SetState("Chase Enemy");
}

void Warrior::Update()
{
	// Chase Enemy
	if (WarriorMobDist > 2 && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Chase Enemy");

	// Attack
	if (WarriorMobDist <= 2 && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Attack");

	// Knockback
	if (AllyMobDist <= 3 && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Knockback");

	// Revive
	if (DeadAlly)
		WarriorSM.SetState("Revive");

	// Dead
	if (HP <= 0)
		Dead = true;
	else
		Dead = false;
	if (Dead)
		WarriorSM.SetState("Dead");

	// States
	if (WarriorSM.GetState() == "Chase Enemy")
	{
		
	}
	else if (WarriorSM.GetState() == "Attack")
	{

	}
	else if (WarriorSM.GetState() == "Knockback")
	{
		
	}
	else if (WarriorSM.GetState() == "Revive")
	{
		// Move to dead ally and revive them
	}
	else if (WarriorSM.GetState() == "Dead")
	{
		// Immobilized until teammate revives him
	}
}

void Warrior::Delete()
{

}