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
	SetPosition(Vector3(0, -40, 0));
	Speed = 6.f;
	AttackRange = 2.f;
	Attack = true;
	Cooldown = 0.f;
	Dead = false;
	DeadAlly = false;
	WarriorSM.AddState("Chase Enemy");
	WarriorSM.AddState("Attack");
	WarriorSM.AddState("Knockback");
	WarriorSM.AddState("Revive");
	WarriorSM.AddState("Dead");

	WarriorSM.SetState("Chase Enemy");
}

void Warrior::Update(double dt)
{
	// Chase Enemy
	if (WarriorMobDist > AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Chase Enemy");

	// Attack
	if (WarriorMobDist <= AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Attack");

	// Knockback
	if (AllyMobDist <= AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
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

	// Attack Cooldown
	if (!Attack)
	{
		Cooldown += dt;
	}
	if (Cooldown >= 1.f)
	{
		Attack = true;
		Cooldown = 0;
	}

	// States
	if (WarriorSM.GetState() == "Chase Enemy")
	{
		Vector3 temp;
		temp = EManager.FindNearestEntity_Pos(Position, "Monster");
		Position += (Position - temp) * Speed * dt;
	}
	else if (WarriorSM.GetState() == "Attack")
	{
		if (Attack)
		{
			EManager.DecreaseEntityHP("Monster", Math::RandIntMinMax(5, 15));
			Attack = false;
		}
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