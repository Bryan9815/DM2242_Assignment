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

void Warrior::Init(EntityManager* EManager, float world_width, float world_height)
{
    this->EManager = EManager;

	SetPosition(Vector3(20, 50, 0));
	Speed = 6.f;
	AttackRange = 6.f;
	Cooldown = 2.f;
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

void Warrior::Init(EntityManager* EManager, float world_width, float world_height, Vector3 startpos)
{
	this->EManager = EManager;

	SetPosition(Vector3(20, 50, 0));
	Speed = 6.f;
	AttackRange = 2.f;
	Cooldown = 2.f;
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
	//WrapAroundScreen();
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
	{
		Dead = true;
		WarriorSM.SetState("Dead");
	}	

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
		if (Cooldown >= 0.7f)
		{
			int temp = 0;
			temp = Math::RandIntMinMax(6, 15);
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
			EManager->IncreaseEntityAggro("Warrior", 50);
			Mob->MobSM.SetState("Knocked Back");
			WarriorSM.SetState("Chase Enemy");
		}
	}
	else if (WarriorSM.GetState() == "Revive")
	{
		BaseEntity* tempEntity = EManager->GetNearestDeadHero(Name);

		if ((Position - tempEntity->GetPosition()).Length() > ReviveRange)
		{
			Position += (tempEntity->GetPosition() - Position).Normalize() * Speed * dt;
			return;
		}
		if (tempEntity->GetName() == "Warrior")
		{
			Warrior* tempWarrior = dynamic_cast<Warrior*>(tempEntity);
			tempWarrior->SetHP(100);
		}
		else if (tempEntity->GetName() == "Healer")
		{

		}
	}
	else if (WarriorSM.GetState() == "Dead")
	{
		Aggro = 0;
		if (HP > 0)
		{
			Dead = false;
			WarriorSM.SetState("Chase Enemy");
		}
	}
}

void Warrior::WrapAroundScreen()
{
#define OFFSET (scale * 0.5f)

	if (Position.x > world_width + OFFSET)
		Position.x = -OFFSET;
	else if (Position.x < 0 - OFFSET)
		Position.x = world_width + OFFSET;

	if (Position.y > world_height + OFFSET)
		Position.y = -OFFSET;
	else if (Position.y < -OFFSET)
        Position.y = world_height + OFFSET;
}

void Warrior::Delete()
{

}