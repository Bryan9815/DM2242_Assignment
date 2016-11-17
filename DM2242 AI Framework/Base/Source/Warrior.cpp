#include "Warrior.h"
#include "MobEntity.h"

MobEntity *Mob;

Warrior::Warrior() 
{
    Name = "Warrior";    
}

Warrior::~Warrior()
{
	Delete();
}

void Warrior::Init(EntityManager* EManager)
{
    this->EManager = EManager;
    
	HP = 100;
	SetPosition(Vector3(10, 40, 0));
	Speed = 6.f;
	Aggro = 0;
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

	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Mob")//entity to find
		{
			Mob = dynamic_cast<MobEntity*>((*it));
			break;
		}
	}
}

void Warrior::Init(EntityManager* EManager, Vector3 startpos)
{
	this->EManager = EManager;

	HP = 100;
	SetPosition(Vector3(10, 40, 0));
	Speed = 6.f;
	Aggro = 0;
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

	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Mob")//entity to find
		{
			Mob = dynamic_cast<MobEntity*>((*it));
			break;
		}
	}
}

void Warrior::Update(double dt)
{
	WarriorMobDist = EManager->FindDistanceBetweenEntities(Position, "Mob");
	RangerMobDist = EManager->FindDistanceBetweenEntities("Ranger", "Mob");
	HealerMobDist = EManager->FindDistanceBetweenEntities("Healer", "Mob");

	// Chase Enemy
    if (WarriorMobDist > AttackRange && WarriorSM.GetState() != "Knocking Back"  && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Chase Enemy");

	// Attack
	if (WarriorMobDist <= AttackRange && WarriorSM.GetState() != "Knocking Back" && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
		WarriorSM.SetState("Attack");

	// Knocking Back
	if ((RangerMobDist <= 3.f && HealerMobDist <= 3.f) && WarriorSM.GetState() != "Revive" && WarriorSM.GetState() != "Dead")
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

	// States
	if (WarriorSM.GetState() == "Chase Enemy")
	{
		Vector3 temp;
		temp = EManager->FindNearestEntity_Pos(Position, "Mob");
		Position += -(Position - temp).Normalize() * Speed * dt;

	}
	else if (WarriorSM.GetState() == "Attack")
	{
		Cooldown += dt;
		if (Cooldown >= 1.f)
		{
			int temp = 0;
			temp = Math::RandIntMinMax(5, 15);
			EManager->DecreaseEntityHP("Mob", temp);
			EManager->IncreaseEntityAggro("Warrior", temp);
			Cooldown = 0;
		}

        /*if (EManager->FindDistanceBetweenEntities("Warrior", "Mob") < AttackRange - 0.2f)
        {
            Vector3 temp;
            temp = EManager->FindNearestEntity_Pos("Warrior", "Mob");
            Position += (Position - temp).Normalize() * Speed * dt;
        }*/
	}
	else if (WarriorSM.GetState() == "Knocking Back")
	{
		if (WarriorMobDist > AttackRange)
		{
			Vector3 temp;
			temp = EManager->FindNearestEntity_Pos(Position, "Mob");
            Position += -(Position - temp).Normalize() * Speed * dt;
        }
		else if (WarriorMobDist <= AttackRange)
		{
			EManager->DecreaseEntityHP("Mob", 5);
			EManager->IncreaseEntityAggro("Warrior", 30);
			Mob->MobSM.SetState("Knocked Back");
			WarriorSM.SetState("Chase Enemy");
		}
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