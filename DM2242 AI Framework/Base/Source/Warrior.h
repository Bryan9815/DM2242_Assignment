#ifndef WARRIOR_H
#define WARRIOR_H

#include "BaseEntity.h"

class Warrior : public BaseEntity
{
public:
	Warrior();
	~Warrior();

	void Init();
	void Update();
	void Delete();
};

#endif