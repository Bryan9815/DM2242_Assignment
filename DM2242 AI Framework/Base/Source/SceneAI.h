#ifndef SCENE_AI_H
#define SCENE_AI_H

#include "GameObject.h"
#include <vector>
#include "SceneBase.h"
#include "RangerEntity.h"
#include "Warrior.h"
#include "MobEntity.h"

class SceneAI : public SceneBase
{
	static const int MAX_SPEED = 20;

public:
	SceneAI();
	~SceneAI();

	virtual void Init();
    void InitEntity();
	virtual void Update(double dt);
	virtual void Render();
    void RenderEntity();
	virtual void Exit();

    void Insert_Text_On_Screen(float x, float y, float size, Color colour, string text);
	void RenderGO(GameObject *go);

	GameObject* FetchGO();

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
	Vector3 m_force;
	unsigned short obstacleCount;

	//Auditing
	float m1, m2;
	Vector3 u1, u2, v1, v2;

    EntityManager* Entity_Manager;
    RangerEntity* Ranger;
    Warrior* warrior;
    MobEntity* mob;
};

#endif