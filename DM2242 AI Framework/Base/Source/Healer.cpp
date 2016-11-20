#include "Healer.h"
#include "Warrior.h"
#include "RangerEntity.h"
#define MAXHP 100
#define STARTPOS Vector3(5,45,0)
#define TIME_BETWEEN_ATTACKS 1.5f
#define REVIVE_RANGE 1.4f
HealerEntity::HealerEntity()
{
}

HealerEntity::~HealerEntity()
{
}

void HealerEntity::Init(EntityManager* Entity_Manager, float world_width, float world_height)
{
    this->world_height = world_height;
    this->world_width = world_width;
    this->Entity_Manager = Entity_Manager;
    SetPosition(STARTPOS);

    //stats
    AttackRange = 4.0f;
    AttackDamage = 3.0f;
    MovementSpeed = 4.0f;
    DeadAlly = false;
    NearestEnemyDist = 0;
    NearestDeadAllyDist = 0;
    NearEnemies = 0;
    AttackReset_Timer = 0;
    InjuredAllyEntity = NULL;
    //states
    HealerSM.Init();
    HealerSM.AddState("Move");
    HealerSM.AddState("Shoot");
    HealerSM.AddState("Death");
    HealerSM.AddState("Revive");
    HealerSM.AddState("Heal");
    //set starting state
    HealerSM.SetState("Move");
    
}

void HealerEntity::Init(EntityManager* Entity_Manager, float world_width, float world_height, Vector3 startpos)
{
    Name = "Ranger";
    SetPosition(startpos);
    this->Entity_Manager = Entity_Manager;


    AttackRange = 4.0f;
    AttackDamage = 3.0f;
    MovementSpeed = 4.f;
    DeadAlly = false;
    NearestEnemyDist = 0;
    NearestDeadAllyDist = 0;
    NearEnemies = 0;
    AttackReset_Timer = 0;
    InjuredAllyEntity = NULL;


    HealerSM.Init();
    HealerSM.AddState("Move");
    HealerSM.AddState("Shoot");
    HealerSM.AddState("Death");
    HealerSM.AddState("Revive");

    HealerSM.SetState("Move");

    
}
void HealerEntity::Update(double dt)
{
    WrapAroundScreen();
    UpdateVariables(dt);
    StateCheck();
    StateRun(dt);
}

void HealerEntity::Delete()
{

}

void HealerEntity::StateCheck()
{
    if (HP <= 0)
    {
        HealerSM.SetState("Death");
        Dead = true;
        Aggro = 0;
        return;
    }
    if (HealerSM.GetState() == "Move")
    {
        if (DeadAlly)
            HealerSM.SetState("Revive");
        if (NearestEnemyDist <= 7)
            HealerSM.SetState("Shoot");
    }
    else if (HealerSM.GetState() == "Shoot")
    {
        if (NearestEnemyDist > 6)
        {
            HealerSM.SetState("Move");
        }
        if (DeadAlly)
            HealerSM.SetState("Revive");

    }
    else if (HealerSM.GetState() == "Revive")
    {
        if (!DeadAlly)
            HealerSM.SetState("Move");
    }
    else if (HealerSM.GetState() == "Heal")
    {
        
    }
    else
    {

    }
}

void HealerEntity::StateRun(double dt)
{
    if (HealerSM.GetState() == "Move")
    {
        if (NearestEnemyDist > AttackRange)
            Position += (Entity_Manager->FindNearestEntity_Pos(Position, "Mob") - Position).Normalize() * MovementSpeed * dt;
        else
            HealerSM.SetState("Shoot");
    }
    else if (HealerSM.GetState() == "Shoot")
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
    /*else if (RangerSM.GetState() == "Bomb")
    {

    }*/
    else if (HealerSM.GetState() == "Death")
    {

    }
    else if (HealerSM.GetState() == "Revive")
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
            HealerSM.SetState("Move");
        }
        else if (tempEntity->GetName() == "Healer")
        {

        }

    }
    else
    {

    }
}


void HealerEntity::UpdateVariables(double dt)
{
    NearestEnemyDist = Entity_Manager->FindDistanceBetweenEntities(Position, "Mob");
    if (AttackReset_Timer < TIME_BETWEEN_ATTACKS)
        AttackReset_Timer += dt;
    BaseEntity* temp;
    DeadAlly = Entity_Manager->Hero_getDead(Name);

    for (vector<BaseEntity*>::iterator it = Entity_Manager->EntityList.begin(); it != Entity_Manager->EntityList.end(); ++it)
    {
        if ((*it)->GetName() != "Mob" && (*it)->GetHP() < 50)
        {
            InjuredAllyEntity = (*it);
            InjuredAlly = true;
        }
    }
}

void HealerEntity::Revive()
{
    if (HealerSM.GetState() == "Death")
    {
        HP = MAXHP;
        HealerSM.SetState("Move");
        SetDead(false);
    }
}

void HealerEntity::WrapAroundScreen()
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