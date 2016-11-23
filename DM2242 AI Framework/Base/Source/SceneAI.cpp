#include "SceneAI.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAI::SceneAI()
{
}

SceneAI::~SceneAI()
{
}

void SceneAI::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	Mtx44 Rotation;
	Rotation.SetToRotation(90, 0, 0, 1);

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	m_ghost = new GameObject(GameObject::GO_BALL);

    Entity_Manager = new EntityManager();
    Entity_Manager->Init();


    Ranger = new RangerEntity();    
    Entity_Manager->AddEntity(Ranger);

    warrior = new Warrior();
    Entity_Manager->AddEntity(warrior);

    Healer = new HealerEntity();
    Entity_Manager->AddEntity(Healer);

    mob = new MobEntity();    
    Entity_Manager->AddEntity(mob);

    Ranger->Init(Entity_Manager, m_worldWidth,m_worldHeight);
	warrior->Init(Entity_Manager, m_worldWidth, m_worldHeight);
    Healer->Init(Entity_Manager, m_worldWidth, m_worldHeight);
	mob->Init(Entity_Manager, m_worldWidth, m_worldHeight);
}

GameObject* SceneAI::FetchGO()
{
	//Exercise 2a: implement FetchGO()
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		if (!(*it)->active)
		{
			(*it)->active = true;
			m_objectCount += 1;
			return (*it);
		}
	}
	// No inactive object found
	for (int i = 0; i < 10; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
	GameObject* go = m_goList.back();;
	go->active = true;
	m_objectCount += 1;
	return go;
}

bool SceneAI::CheckCollision(GameObject *go1, GameObject *go2, float dt)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		if (go1->type == GameObject::GO_BALL && go2->type == GameObject::GO_BALL)
		{
			float distanceSquared = ((go1->pos + go1->vel * dt) - (go2->pos - go2->vel * dt)).LengthSquared();
			float combinedRadiusSquared = (go1->scale.x + go2->scale.x) * (go1->scale.x + go2->scale.x);
			Vector3 relativeVelocity = go1->vel - go2->vel;
			Vector3 relativeDisplacement = go2->pos - go1->pos;
			return distanceSquared < combinedRadiusSquared && relativeVelocity.Dot(relativeDisplacement) > 0;
		}
	}
	case GameObject::GO_WALL:
	{
		Vector3  w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->normal;
		float r = go1->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;

		Vector3 NP(-N.y, N.x);

		Vector3 RV = go1->vel;
		Vector3 RD = w0 - b1;

		if (RD.Dot(N) < 0)
			N = -N;

		return abs(RD.Dot(N)) < r + h / 2 && abs(RD.Dot(NP)) < l / 2 && RV.Dot(N) > 0;
	}
	}
}

void SceneAI::CollisionResponse(GameObject *go1, GameObject *go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		//Exercise 8b: store values in auditing variables
		m1 = go1->mass;
		m2 = go2->mass;
		u1 = go1->vel;
		u2 = go2->vel;

		Vector3 u1N, u2N, N;
		N = (go2->pos - go1->pos).Normalized();

		u1N = u1.Dot(N) * N;
		u2N = u2.Dot(N) * N;
		go1->vel = u1 + (2 * m2) / (m1 + m2) * (u2N - u1N);
		go2->vel = u2 + (2 * m1) / (m1 + m2) * (u1N - u2N);

		v1 = go1->vel;
		v2 = go2->vel;
		break;
	}
	case GameObject::GO_WALL:
	{
		m1 = go1->mass;
		m2 = go2->mass;
		Vector3 u = go1->vel;
		Vector3 N = go2->normal;
		Vector3 uN = u.Dot(N) * N;
		go1->vel = u - 2 * uN;
		go2->active = false;
		m_objectCount -= 1;
		obstacleCount -= 1;
		break;
	}
	}
}

void SceneAI::Update(double dt)
{
	SceneBase::Update(dt);

    //Entity Update
    Entity_Manager->Update(dt);

    if (Application::IsKeyPressed('X'))
    {
        warrior->WarriorSM.SetState("Knocking Back");
    }
}


void SceneAI::RenderGO(GameObject *go)
{
	switch (go->type)
	{
	case GameObject::GO_BALL:
	{	//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_WALL:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
	}
	}
}

void SceneAI::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Projection matrix : Orthographic Projection
	Mtx44 projection;
	projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
	projectionStack.LoadMatrix(projection);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();

    RenderEntity();

}

void SceneAI::RenderEntity()
{
    
    for (vector<BaseEntity*>::iterator it = Entity_Manager->EntityList.begin(); it != Entity_Manager->EntityList.end(); ++it)
    {
        modelStack.PushMatrix();
        if ((*it)->GetName() == "Ranger")
        {
            Vector3 temp = (*it)->GetPosition();
            modelStack.Translate(temp.x, temp.y, temp.z);
            modelStack.Scale((*it)->GetScale(), (*it)->GetScale(), (*it)->GetScale());
            RenderMesh(meshList[GEO_RANGER], false);

            RangerEntity* tempRanger = dynamic_cast<RangerEntity*>((*it));

            Insert_Text_On_Screen(temp.x - 2, temp.y + 3, 2.f, Color(1, 1, 1), (*it)->GetName());//render HP below entity


            Insert_Text_On_Screen(temp.x - 2, temp.y - 3, 2.f, Color(1, 1, 1), to_string((*it)->GetHP()));//render HP below entity
            Insert_Text_On_Screen(temp.x - 2, temp.y - 5, 2.f, Color(1, 1, 1), tempRanger->RangerSM.GetState());//render state
        }
        else if ((*it)->GetName() == "Warrior")
        {
            Vector3 temp = (*it)->GetPosition();
            modelStack.Translate(temp.x, temp.y, temp.z);
            modelStack.Scale((*it)->GetScale(), (*it)->GetScale(), (*it)->GetScale());
            RenderMesh(meshList[GEO_WARRIOR], false);

            Warrior* tempWarrior = dynamic_cast<Warrior*>((*it));

            Insert_Text_On_Screen(temp.x - 2, temp.y + 3, 2.f, Color(1, 1, 1), (*it)->GetName());//render HP below entity


            Insert_Text_On_Screen(temp.x - 2, temp.y - 3, 2.f, Color(1, 1, 1), to_string((*it)->GetHP()));//render HP below entity
            Insert_Text_On_Screen(temp.x - 2, temp.y - 5, 2.f, Color(1, 1, 1), tempWarrior->WarriorSM.GetState());//render state

        }
        else if ((*it)->GetName() == "Healer")
        {
            Vector3 temp = (*it)->GetPosition();
            modelStack.Translate(temp.x, temp.y, temp.z);
            modelStack.Scale((*it)->GetScale(), (*it)->GetScale(), (*it)->GetScale());
            RenderMesh(meshList[GEO_HEALER], false);

            HealerEntity* tempHealer = dynamic_cast<HealerEntity*>((*it));

            Insert_Text_On_Screen(temp.x - 2, temp.y + 3, 2.f, Color(1, 1, 1), (*it)->GetName());//render HP below entity


            Insert_Text_On_Screen(temp.x - 2, temp.y - 3, 2.f, Color(1, 1, 1), to_string((*it)->GetHP()));//render HP below entity
            Insert_Text_On_Screen(temp.x - 2, temp.y - 5, 2.f, Color(1, 1, 1), tempHealer->HealerSM.GetState());//render state

        }
        else if ((*it)->GetName() == "Mob")
        {
            Vector3 temp = (*it)->GetPosition();
            modelStack.Translate(temp.x, temp.y, temp.z);
            modelStack.Scale((*it)->GetScale(), (*it)->GetScale(), (*it)->GetScale());
            RenderMesh(meshList[GEO_MOB], false);

            MobEntity* tempMob = dynamic_cast<MobEntity*>((*it));

            Insert_Text_On_Screen(temp.x - 2, temp.y + 5, 2.f, Color(1, 1, 1), (*it)->GetName());//render HP below entity


            Insert_Text_On_Screen(temp.x - 2, temp.y - 5, 2.f, Color(1, 1, 1), to_string((*it)->GetHP()));//render HP below entity
            Insert_Text_On_Screen(temp.x - 2, temp.y - 8, 2.f, Color(1, 1, 1), tempMob->MobSM.GetState());//render state
        }
        else
        {

        }
        modelStack.PopMatrix();
    }
    
}

void SceneAI::Insert_Text_On_Screen(float x, float y, float size, Color colour, string text)
{
    modelStack.PushMatrix();
    Mtx44 projection;
    projection.SetToOrtho(0, m_worldWidth, 0, m_worldHeight, -10, 10);
    RenderTextOnScreen(meshList[GEO_TEXT], text, colour, size, x, y, projection);
    modelStack.PopMatrix();
}

void SceneAI::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if (m_ghost)
	{
		delete m_ghost;
		m_ghost = NULL;
	}
}
