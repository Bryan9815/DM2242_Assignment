#include "Warrior.h"
#include "MobEntity.h"

Warrior::Warrior()
{

}

Warrior::~Warrior()
{
	Delete();
}

void Warrior::Init(EntityManager* EManager)
{
    this->EManager = EManager;
    Name = "Warrior";
	HP = 100;
	SetPosition(Vector3(0, 40, 0));
	Speed = 6.f;
	AttackRange = 2.f;
	Attack = true;
	Cooldown = 0.f;
	Dead = false;
	DeadAlly = false;
	WarriorSM.AddState("Chase Enemy");
	WarriorSM.AddState("Attack");
	WarriorSM.AddState("Knocking Back");
	WarriorSM.AddState("Revive");
	WarriorSM.AddState("Dead");

	WarriorSM.SetState("Chase Enemy");
}

void Warrior::Init(EntityManager* EManager, Vector3 startpos)
{
    this->EManager = EManager;
    Name = "Warrior";
    HP = 100;
    SetPosition(startpos);
    Speed = 6.f;
    AttackRange = 2.f;
    Attack = true;
    Cooldown = 0.f;
    Dead = false;
    DeadAlly = false;
    WarriorSM.AddState("Chase Enemy");
    WarriorSM.AddState("Attack");
    WarriorSM.AddState("Knocking Back");
    WarriorSM.AddState("Revive");
    WarriorSM.AddState("Dead");

    WarriorSM.SetState("Chase Enemy");
}

void Warrior::Update(double dt)
{
	WarriorMobDist = EManager->FindDistanceBetweenEntities(Position, "Mob");
	
	// Chase Enemy
	if (WarriorMobDist > AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Chase Enemy");

	// Attack
	if (WarriorMobDist <= AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Attack");

	// Knocking Back
	if (AllyMobDist <= AttackRange && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Knocking Back");

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
		temp = EManager->FindNearestEntity_Pos(Position, "Mob");
		Position += (Position - temp) * Speed * dt;
	}
	else if (WarriorSM.GetState() == "Attack")
	{
		if (Attack)
		{
			int temp = 0;
			temp = Math::RandIntMinMax(5, 15);
			EManager->DecreaseEntityHP("Mob", temp);
			EManager->IncreaseEntityAggro("Warrior", temp);
			Attack = false;
		}
	}
	else if (WarriorSM.GetState() == "Knocking Back")
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