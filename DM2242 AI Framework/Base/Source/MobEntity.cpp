#include "MobEntity.h"
#include "Warrior.h"

MobEntity::MobEntity()
{

}


MobEntity::~MobEntity()
{
    Delete();
}
void MobEntity::Init(EntityManager* EManager)
{
    Name = "Mob";
    HP = 300;
    Dead = false;
	Attack = true;
	SetPosition(Vector3(50, 50, 0));
	Speed = 5.f;
	AttackRange = 1.5f;
	Cooldown = 0.f;

	Target = "";

	//States
    MobSM.AddState("Chase Target");
    MobSM.AddState("Attack");
	MobSM.AddState("Knocked Back");
    MobSM.AddState("Stunned");
    MobSM.AddState("Dead");

    MobSM.SetState("Chase Target");
}

void MobEntity::Init(EntityManager* EManager, Vector3 startpos)
{
    Name = "Mob";
    HP = 300;
    Dead = false;
    Attack = true;
    SetPosition(startpos);
    Speed = 5.f;
    AttackRange = 1.5f;
    Cooldown = 0.f;

    Target = "";

    //States
    MobSM.AddState("Chase Target");
    MobSM.AddState("Attack");
	MobSM.AddState("Knocked Back");
	MobSM.AddState("Stunned");
    MobSM.AddState("Dead");

    MobSM.SetState("Chase Target");
}

void MobEntity::DetermineTarget()
{
	// if aggro equal, go after closest target, else go after highest aggro

}

void MobEntity::Update(double dt)
{
	DetermineTarget();
	// Chase Target
	if (DistFromTarget > 1.5f)
		MobSM.SetState("Chase Target");

	// Attack
	if (DistFromTarget <= 1.5f)
		MobSM.SetState("AttacK");

	// Stunned
	/*if (WarriorSkill)
	{
		float temp = 0;
		temp = Math::RandFloatMinMax(0, 101);
		if (temp >= 46)
		{
			MobSM.SetState("Stunned");
			WarriorAggro += 50;
		}
		else
			WarriorAggro += 25;
		WarriorSkill = false;
	}*/

	// Dead
	if (HP <= 0)
		MobSM.SetState("Dead");

	// Attack Cooldown
	if (!Attack)
	{
		Cooldown += dt;
	}
	if (Cooldown >= 2.f)
	{
		Attack = true;
		Cooldown = 0;
	}

	// Stun Timer
	if (Stunned)
		StunTimer += dt;
	if (StunTimer >= 3.f)
	{
		Stunned = false;
		MobSM.SetState("Chase Target");
		StunTimer = 0;
	}

	// States
    if (MobSM.GetState() == "Chase Target")
    {
		Vector3 temp;
		temp = EManager->FindNearestEntity_Pos(Position, Target);
		Position += (Position - temp) * Speed * dt;
    }
    else if (MobSM.GetState() == "Attack")
    {
		if (Attack)
		{
			EManager->DecreaseEntityHP(Target, Math::RandIntMinMax(20, 30));
			Attack = false;
		}
    }
	else if (MobSM.GetState() == "Knocked Back")
	{

	}
	else if (MobSM.GetState() == "Stunned")
	{
		Stunned = true;
	}
    else if (MobSM.GetState() == "Dead")
    {
		// Immobilized
    }
}

void MobEntity::Delete()
{

}
