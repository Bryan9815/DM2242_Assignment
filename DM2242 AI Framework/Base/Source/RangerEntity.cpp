#include "RangerEntity.h"


RangerEntity::RangerEntity()
{
}


RangerEntity::~RangerEntity()
{
}
void RangerEntity::Init()
{
    HP = 100;
    Dead = false;
    DeadAlly = false;
}
void RangerEntity::Update()
{

}
int  RangerEntity::GetHP()
{
    return HP;
}
void RangerEntity::SetHP(int hp)
{
    HP = hp;
}
bool RangerEntity::GetDead()
{
    return Dead;
}
void RangerEntity::SetDead(bool dead)
{
    Dead = dead;
}
void RangerEntity::Delete()
{

}
void RangerEntity::SetPosition(Vector3 pos)
{

}
Vector3 GetPosition()
{

}