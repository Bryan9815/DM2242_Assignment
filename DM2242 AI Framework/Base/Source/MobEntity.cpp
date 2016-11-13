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
    HP = 100;
    Dead = false;
    SetPosition(Vector3(0, 0, 0));
    MobSM.Init();
    //states
    MobSM.AddState("Move");
    MobSM.AddState("Attack");
    MobSM.AddState("Death");

    MobSM.SetState("Move");
}
void MobEntity::Update()
{
    if (MobSM.GetState() == "Move")
    {

    }
    else if (MobSM.GetState() == "Attack")
    {

    }
    else if (MobSM.GetState() == "Death")
    {

    }
    else
    {

    }
}

void MobEntity::Delete()
{

}
