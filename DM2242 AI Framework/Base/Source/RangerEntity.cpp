#include "RangerEntity.h"
#define MAXHP 100
#define STARTPOS Vector3(10,45,0)
#define TIME_BETWEEN_ATTACKS 1.0f

RangerEntity::RangerEntity()
{
}


RangerEntity::~RangerEntity()
{
    Delete();
}
void RangerEntity::Init(EntityManager* Entity_Manager)
{
    Name = "Ranger";
    SetPosition(STARTPOS);
    this->Entity_Manager = Entity_Manager;
    AttackRange = 5.0f;
    AttackDamage = 5.0f;
    MovementSpeed = 5.0f;
    HP = MAXHP;
    Dead = false;
    DeadAlly = false;
    NearestEnemyDist = 0;
    NearestDeadAllyDist = 0;
    NearEnemies = 0;
    RangerSM.Init();
    RangerSM.AddState("Move");
    RangerSM.AddState("Shoot");
    RangerSM.AddState("Death"); 
    RangerSM.AddState("Bomb");
    RangerSM.AddState("Revive");

    RangerSM.SetState("Move");

    AttackReset_Timer = 0;
}

void RangerEntity::Init(EntityManager* Entity_Manager, Vector3 startpos)
{
    Name = "Ranger";
    SetPosition(startpos);
    this->Entity_Manager = Entity_Manager;
    AttackRange = 5.0f;
    AttackDamage = 5.0f;
    MovementSpeed = 5.f;
    HP = MAXHP;
    Dead = false;
    DeadAlly = false;
    NearestEnemyDist = 0;
    NearestDeadAllyDist = 0;
    NearEnemies = 0;
    RangerSM.Init();
    RangerSM.AddState("Move");
    RangerSM.AddState("Shoot");
    RangerSM.AddState("Death");
    RangerSM.AddState("Bomb");
    RangerSM.AddState("Revive");

    RangerSM.SetState("Move");

    AttackReset_Timer = 0;
}
void RangerEntity::Update(double dt)
{
    UpdateVariables(dt);
    StateCheck();
    StateRun(dt);
}

void RangerEntity::Delete()
{

}

void RangerEntity::StateCheck()
{
    if (HP <= 0)
    {
        RangerSM.SetState("Death");
        Dead = true;
        return;
    }
        
    if (RangerSM.GetState() == "Move")
    {        
        if (DeadAlly)
            RangerSM.SetState("Revive");        
        if (NearestEnemyDist <= 5)
            RangerSM.SetState("Shoot");
    }
    else if (RangerSM.GetState() == "Shoot")
    {
        if (NearestEnemyDist < 5)
            RangerSM.SetState("Move");
        /*if (NearEnemies >= 4)
            RangerSM.SetState("Bomb");*/
    }
    /*else if (RangerSM.GetState() == "Bomb")
    {
        if (NearEnemies < 4)
            RangerSM.SetState("Shoot");
    }*/
    else if (RangerSM.GetState() == "Death")
    {
        
    }    
    else if (RangerSM.GetState() == "Revive")
    {
        if (!DeadAlly)
            RangerSM.SetState("Move");
    }
    else
    {

    }
}

void RangerEntity::Revive()
{
    if (RangerSM.GetState() == "Death")
    {
        HP = MAXHP;
        RangerSM.SetState("Move");
    }    
}

void RangerEntity::StateRun(double dt)
{
    if (RangerSM.GetState() == "Move")
    {
        if (NearestEnemyDist > AttackRange)
            Position += (Entity_Manager->FindNearestEntity_Pos(Position, "Mob") - Position).Normalize() * MovementSpeed * dt;
        else
            RangerSM.SetState("Shoot");
    }
    else if (RangerSM.GetState() == "Shoot")
    {
        if (AttackReset_Timer > TIME_BETWEEN_ATTACKS)
        {
            AttackReset_Timer = 0;
            Entity_Manager->DecreaseEntityHP("Mob", AttackDamage);
        }
    }
    /*else if (RangerSM.GetState() == "Bomb")
    {
        
    }*/
    else if (RangerSM.GetState() == "Death")
    {

    }
    else if (RangerSM.GetState() == "Revive")
    {
        
    }
    else
    {

    }
}


void RangerEntity::UpdateVariables(double dt)
{
    NearestEnemyDist = Entity_Manager->FindDistanceBetweenEntities(Position, "Mob");
    if (AttackReset_Timer < TIME_BETWEEN_ATTACKS)
        AttackReset_Timer += dt;
    
}