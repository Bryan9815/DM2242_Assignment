#include "MobEntity.h"
#include "Warrior.h"
#include "RangerEntity.h"

Warrior *warrior;
RangerEntity *ranger;

MobEntity::MobEntity()
{
    Name = "Mob";
}


MobEntity::~MobEntity()
{
    Delete();
}
void MobEntity::Init(EntityManager* EManager, float world_width, float world_height)
{
    this->EManager = EManager;
    HP = 3000;
	Stunned = false;
	SetPosition(Vector3(50, 50, 0));
	this->world_height = world_height;
	this->world_width = world_width;
	Speed = 5.f;
	scale = 4.f;
	AttackRange = 6.f;
	Cooldown = 0.f;
    knockTimer = 0;
	StunTimer = 0;
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
	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Ranger")//entity to find
		{
			ranger = dynamic_cast<RangerEntity*>((*it));
			break;
		}
	}
}

void MobEntity::Init(EntityManager* EManager, float world_width, float world_height, Vector3 startpos)
{
	this->EManager = EManager;
	HP = 300;
	Stunned = false;
	SetPosition(Vector3(50, 50, 0));
	this->world_height = world_height;
	this->world_width = world_width;
	Speed = 5.f;
	scale = 2.f;
	AttackRange = 1.5f;
	Cooldown = 0.f;
	knockTimer = 0;
	StunTimer = 0;
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
	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
		if ((*it)->GetName() == "Ranger")//entity to find
		{
			ranger = dynamic_cast<RangerEntity*>((*it));
			break;
		}
	}
}

void MobEntity::DetermineTarget()
{
	// if aggro equal, go after closest target, else go after highest aggro
	int temp_Aggro = 0;
	float temp_Dist = 0;
    bool temp = false;
	for (vector<BaseEntity*>::iterator it = EManager->EntityList.begin(); it != EManager->EntityList.end(); ++it)
	{
        if ((*it)->GetName() != "Mob")
        {
            if (!(*it)->GetDead())
            {
                if (!temp)
                {
                    temp_Aggro = (*it)->GetAggro();
					temp_Dist = EManager->FindDistanceBetweenEntities(Position, (*it)->GetName());
					Target = (*it)->GetName();
                    temp = true;
                }                    
                else
                {
                    if ((*it)->GetAggro() > temp_Aggro)
                    {
                        temp_Aggro = (*it)->GetAggro();
						temp_Dist = EManager->FindDistanceBetweenEntities(Position, (*it)->GetName());
                        Target = (*it)->GetName();
                    }
                    else if ((*it)->GetAggro() == temp_Aggro)
                    {
                        //if aggro is the same
						if (EManager->FindDistanceBetweenEntities(Position, (*it)->GetName()) < temp_Dist)
							Target = (*it)->GetName();
                    }
                }
            }
        }
	}
    if (!temp) // if cant find
        Target = "";
}

void MobEntity::Update(double dt)
{
	WrapAroundScreen();
	DetermineTarget();
    DistFromTarget = EManager->FindDistanceBetweenEntities(Position, Target);
	// Chase Target
	if (DistFromTarget > AttackRange && MobSM.GetState() != "Knocked Back" && MobSM.GetState() != "Stunned")
		MobSM.SetState("Chase Target");

	// Attack
    if (DistFromTarget <= AttackRange && MobSM.GetState() != "Knocked Back" && MobSM.GetState() != "Stunned")
		MobSM.SetState("Attack");

	// Dead
	if (HP <= 0)
		MobSM.SetState("Dead");

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
        Position += (temp - Position).Normalize() * Speed * dt;
    }
    else if (MobSM.GetState() == "Attack")
    {
		Cooldown += dt;
		if (Cooldown >= 2.f)
		{
			EManager->DecreaseEntityHP(Target, Math::RandIntMinMax(20, 30));
			Cooldown = 0;
		}        
    }
	else if (MobSM.GetState() == "Knocked Back")
	{
		Vector3 dirVec;
		dirVec = (Position - warrior->GetPosition()).Normalize();
		knockTimer += dt;
		if (knockTimer < 0.25f)
			Position += dirVec * Speed * 4 * dt;
		else
		{
			float stunChance = 0;
			stunChance = Math::RandFloatMinMax(1, 100);
			if (stunChance >= 34)
			{
				MobSM.SetState("Stunned");                
			}
            else
            {
                MobSM.SetState("Chase Target");                
            }				
            knockTimer = 0;
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

void MobEntity::WrapAroundScreen()
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

void MobEntity::Delete()
{

}