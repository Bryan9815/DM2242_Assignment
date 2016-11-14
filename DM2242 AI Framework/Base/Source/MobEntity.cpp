#include "MobEntity.h"


MobEntity::MobEntity()
{

}


MobEntity::~MobEntity()
{
    Delete();
}
void MobEntity::Init()
{
    HP = 300;
    Dead = false;
    SetPosition(Vector3(0, 0, 0));

	Target = "";

	WarriorAggro = 0;
	HealerAggro = 0;
	RangerAggro = 0;

	WarriorTrigger = false;
	HealerTrigger = false;
	RangerTrigger = false;

	WarriorKill = false;
	HealerKill = false;
	RangerKill = false;

	//States
    MobSM.AddState("Chase Target");
    MobSM.AddState("Attack");
    MobSM.SetState("Stunned");
    MobSM.AddState("Dead");
}

void MobEntity::DetermineTarget()
{
	if (WarriorAggro == HealerAggro == RangerAggro)
	{
		// Go after closest target
	}
	// If 1 Hero has the highest Aggro
	else if (WarriorAggro > HealerAggro && WarriorAggro > RangerAggro)
		Target == "Warrior";
	else if (HealerAggro > WarriorAggro && HealerAggro > RangerAggro)
		Target == "Healer";
	else if (RangerAggro > WarriorAggro && RangerAggro > HealerAggro)
		Target == "Ranger";
}

void MobEntity::Update()
{
	DetermineTarget();
	// Chase Target
	if (DistFromTarget > 1.5f)
		MobSM.SetState("Chase Target");

	// Attack
	if (DistFromTarget <= 1.5f)
		MobSM.SetState("AttacK");

	// Stunned
	if (WarriorSkill)
	{
		float temp;
		temp = Math::RandFloatMinMax(0, 101);
		if (temp >= 46)
		{
			MobSM.SetState("Stunned");
			WarriorAggro += 25;
		}
		WarriorSkill = false;
	}

	// Dead
	if (HP <= 0)
		MobSM.SetState("Dead");

	// States
    if (MobSM.GetState() == "Chase Target")
    {
		
    }
    else if (MobSM.GetState() == "Attack")
    {

    }
	else if (MobSM.GetState() == "Stunned")
	{
		// Immobilized for 2 seconds
	}
    else if (MobSM.GetState() == "Dead")
    {
		// Immobilized
    }
}

void MobEntity::Delete()
{

}
