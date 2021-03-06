#include "RangerEntity.h"
#include "Warrior.h"
#define MAXHP 100
#define STARTPOS Vector3(10,45,0)
#define TIME_BETWEEN_ATTACKS 1.0f
#define REVIVE_RANGE 0.7f
#define TIME_BETWEEN_DODGE 5.f
#define DODGE_TIME 1.5f
RangerEntity::RangerEntity()
{
    Name = "Ranger";
}


RangerEntity::~RangerEntity()
{
    Delete();
}
void RangerEntity::InitStats()
{
    AttackRange = 5.0f;
    AttackDamage = 5.0f;
    MovementSpeed = 3.0f;
    DeadAlly = false;
    NearestEnemyDist = 0;
    NearestDeadAllyDist = 0;
    NearEnemies = 0;
    DodgeReset_Timer = 0;
    DodgeSpeed = 10.f;
    DodgeDuration = 0.f;
}
void RangerEntity::Init(EntityManager* Entity_Manager, float world_width, float world_height)
{
    this->world_height = world_height;
    this->world_width = world_width;
    SetPosition(STARTPOS);
    this->Entity_Manager = Entity_Manager;
    InitStats();
    RangerSM.Init();
    RangerSM.AddState("Move");
    RangerSM.AddState("Shoot");
    RangerSM.AddState("Death"); 
    RangerSM.AddState("Bomb");
    RangerSM.AddState("Revive");
    RangerSM.AddState("Dodge");

    RangerSM.SetState("Move");

    AttackReset_Timer = 0;
}

void RangerEntity::Init(EntityManager* Entity_Manager, float world_width, float world_height, Vector3 startpos)
{
    Name = "Ranger";
    SetPosition(startpos);
    this->Entity_Manager = Entity_Manager;
    InitStats();
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
    WrapAroundScreen();
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
        Aggro = 0;
        return;
    }
        
    if (RangerSM.GetState() == "Move")
    {        
        if (DeadAlly)
        {
            RangerSM.SetState("Revive");
            return;
        }
        if (NearestEnemyDist <= 7)
        {
            RangerSM.SetState("Shoot");
            return;
        }
            
    }
    else if (RangerSM.GetState() == "Shoot")
    {
        if (NearestEnemyDist > 7)
        {
            RangerSM.SetState("Move");
            return;
        }
        if (NearestEnemyDist < 5 && (DodgeReset_Timer > TIME_BETWEEN_DODGE))
        {
            RangerSM.SetState("Dodge");
            return;
        }
        if (DeadAlly)
        {
            RangerSM.SetState("Revive");
            return;
        }
            

    }
    else if (RangerSM.GetState() == "Dodge")
    {
        if (NearestEnemyDist > 5 && DodgeReset_Timer < TIME_BETWEEN_DODGE)
        {
            RangerSM.SetState("Shoot");
            return;
        }
            
    }
    else if (RangerSM.GetState() == "Death")
    {
        
    }    
    else if (RangerSM.GetState() == "Revive")
    {
        if (!DeadAlly)
        {
            RangerSM.SetState("Move");
            return;
        }
            
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
        if (NearestEnemyDist < 6.5)
        {
            Position += -(Entity_Manager->FindNearestEntity_Pos(Position, "Mob") - Position).Normalize() * MovementSpeed * dt;
        }
        if (AttackReset_Timer > TIME_BETWEEN_ATTACKS)
        {
            AttackReset_Timer = 0;
            Entity_Manager->DecreaseEntityHP("Mob", AttackDamage);
            Aggro += AttackDamage;
        }
    }
    else if (RangerSM.GetState() == "Dodge")
    {
        if (DodgeReset_Timer > TIME_BETWEEN_DODGE)// dodge off cooldown
        {
            if (DodgeDuration < DODGE_TIME)
            {
                DodgeDuration += dt;
                Position += -(Entity_Manager->FindNearestEntity_Pos(Position, "Mob") - Position).Normalize() * DodgeSpeed * dt;
                return;
            }
            else if (DodgeDuration >= DODGE_TIME)
            {
                DodgeDuration = 0;
                DodgeReset_Timer = 0;
                return;
            }

        }
    }
    else if (RangerSM.GetState() == "Death")
    {

    }
    else if (RangerSM.GetState() == "Revive")
    {
        BaseEntity* tempEntity = Entity_Manager->GetNearestDeadHero(Name);
        
        if ((Position - tempEntity->GetPosition()).Length() > REVIVE_RANGE)
        {
            Position += (tempEntity->GetPosition() - Position).Normalize() * MovementSpeed * dt;
            return;
        }

        if (tempEntity->GetName() == "Warrior")
        {
            Warrior* tempWarrior = dynamic_cast<Warrior*>(tempEntity);
            tempWarrior->SetHP(100);
            RangerSM.SetState("Move");
        }
        else if (tempEntity->GetName() == "Healer")
        {

        }
   
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
    BaseEntity* temp;
    DeadAlly = Entity_Manager->Hero_getDead(Name);
    if (RangerSM.GetState() != "Dodge")
        DodgeReset_Timer += dt;
}

void RangerEntity::WrapAroundScreen()
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