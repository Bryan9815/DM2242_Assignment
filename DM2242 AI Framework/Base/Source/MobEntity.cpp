#include "MobEntity.h"
#include "Warrior.h"

Warrior *warrior;

MobEntity::MobEntity()
{

}


MobEntity::~MobEntity()
{
    Delete();
}
void MobEntity::Init(EntityManager* EManager)
{
    this->EManager = EManager;
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

	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Warrior")//entity to find
		{
			warrior = dynamic_cast<Warrior*>((*it));
			break;
		}
	}
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

	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Warrior")//entity to find
		{
			warrior = dynamic_cast<Warrior*>((*it));
			break;
		}
	}
}

void MobEntity::DetermineTarget()
{
	// if aggro equal, go after closest target, else go after highest aggro
	if (EManager->FindEntityAggro("Warrior") == EManager->FindEntityAggro("Ranger") && EManager->FindEntityAggro("Healer"))
	{
		if (warrior->WarriorMobDist > warrior->HealerMobDist && warrior->WarriorMobDist > warrior->RangerMobDist)
			Target = "Warrior";
		if (warrior->RangerMobDist > warrior->HealerMobDist && warrior->RangerMobDist > warrior->WarriorMobDist)
			Target = "Ranger";
		if (warrior->HealerMobDist > warrior->WarriorMobDist && warrior->HealerMobDist > warrior->RangerMobDist)
			Target = "Healer";
	}
	else if (EManager->FindEntityAggro("Warrior") >= EManager->FindEntityAggro("Ranger") && EManager->FindEntityAggro("Warrior") >= EManager->FindEntityAggro("Healer"))
		Target = "Warrior";
	else if (EManager->FindEntityAggro("Ranger") >= EManager->FindEntityAggro("Warrior") && EManager->FindEntityAggro("Ranger") >= EManager->FindEntityAggro("Healer"))
		Target = "Ranger";
	else if (EManager->FindEntityAggro("Healer") >= EManager->FindEntityAggro("Warrior") && EManager->FindEntityAggro("Healer") >= EManager->FindEntityAggro("Ranger"))
		Target = "Ranger";
	else
		Target = "Warrior";
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
		float knockTimer = 0;
		Vector3 dirVec;
		dirVec = (Position - warrior->GetPosition()).Normalize();
		knockTimer += dt;
		if (knockTimer < 0.5f)
			Position += (Position + dirVec) * 7.5f * dt;
		else
		{
			float stunChance = 0;
			stunChance = Math::RandFloatMinMax(1, 100);
			if (stunChance >= 46)
			{
				MobSM.SetState("Stunned");
			}
			else
				MobSM.SetState("Chase Target");
		}
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

