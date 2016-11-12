#ifndef SCENE_ASSIGNMENT2_H
#define SCENE_ASSIGNMENT2_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"

class SceneAssignment2 : public SceneBase
{
	static const int MAX_SPEED = 20;

public:
	SceneAssignment2();
	~SceneAssignment2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void RenderGO(GameObject *go);

	GameObject* FetchGO();

	void CreatePillarGO(Vector3 wallScale, float rAngle, Vector3 centerPos);

	bool CheckCollision(GameObject *go1, GameObject *go2, float dt);
	void CollisionResponse(GameObject *go1, GameObject *go2);
protected:

	//Physics
	std::vector<GameObject *> m_goList;
	float m_speed;
	float m_worldWidth;
	float m_worldHeight;
	GameObject *m_ghost;
	GameObject *m_ball;
	GameObject *m_bouncer;
	GameObject *m_pillarL;
	GameObject *m_pillarR;
	int m_objectCount;
	unsigned short gameState;
	float selectX;
	float selectY;
	Vector3 m_force;
	unsigned short obstacleCount;
	bool ballSpawn;
	short score;
	short highScore;
	short ballCount;
	unsigned short lives;
	bool easyMode;
	bool power1;
	float powerDuration1;
	float powerTimer1;
	unsigned short powerCount1;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;
};

#endif