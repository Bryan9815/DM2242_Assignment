#include "BaseEntity.h"


BaseEntity::BaseEntity()
{
}


BaseEntity::~BaseEntity()
{
}

int BaseEntity::GetHP()
{
	return HP;
}

void BaseEntity::SetHP(int hp)
{
	HP = hp;
}

bool BaseEntity::GetDead()
{
	return Dead;
}

void BaseEntity::SetDead(bool dead)
{
	Dead = dead;
}

Vector3 BaseEntity::GetPosition()
{
	return Position;
}

void BaseEntity::SetPosition(Vector3 pos)
{
	Position = pos;
}

void BaseEntity::SetName(std::string Name)
{
    this->Name = Name;
}
std::string BaseEntity::GetName()
{
    return Name;
}