#include "RangerEntity.h"


RangerEntity::RangerEntity()
{
}


RangerEntity::~RangerEntity()
{
    Delete();
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

void RangerEntity::Delete()
{

}
