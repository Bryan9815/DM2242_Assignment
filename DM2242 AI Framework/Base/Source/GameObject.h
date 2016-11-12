#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Vector3.h"

struct GameObject
{
	enum GAMEOBJECT_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,
		GO_WALL,
		GO_BORDER,
		GO_BOUNCEWALL,
		GO_PILLAR,
		GO_BOUNCEPILLAR,
		GO_POWERUP1,
		GO_ASTEROID, //asteroid
		GO_SHIP, //player ship
		GO_BULLET, //player bullet
		GO_MISSILE, //player missile
		GO_ESHIP, //Enemy ship
		GO_TOTAL, //must be last
	};
	GAMEOBJECT_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 normal;
	bool active;
	float mass;

	GameObject(GAMEOBJECT_TYPE typeValue = GO_BALL);
	~GameObject();
};

#endif