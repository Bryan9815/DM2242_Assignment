#include "SceneAssignment2.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAssignment2::SceneAssignment2()
{
}

SceneAssignment2::~SceneAssignment2()
{
}

void SceneAssignment2::Init()
{
	SceneBase::Init();

	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	gameState = 0; // 0 = Title Screen, 1 = Instructions, 2 = Game, 3 = Game Over Screen
	selectX = 75;
	selectY = 33.3;
	ballSpawn = false;
	obstacleCount = 0;
	score = 0;
	ballCount = 0;
	lives = 3;
	easyMode = false;
	power1 = false;
	powerTimer1 = 0;
	powerDuration1 = 0;
	powerCount1 = 0;

	//Physics code here
	m_speed = 1.f;
	Mtx44 Rotation;
	Rotation.SetToRotation(90, 0, 0, 1);

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	m_ghost = new GameObject(GameObject::GO_BALL);

	for (int i = 0; i < 60; i += 15)
	{
		for (int j = 0; j < 35; j += 5)
		{
			CreatePillarGO(Vector3(3, 10, 1), 90, Vector3(m_worldWidth / 2 - 22.5 + i, m_worldHeight - 20 - j, 0));
			obstacleCount += 1;
		}
	}

	GameObject *borderT = FetchGO();
	borderT->type = GameObject::GO_BORDER;
	borderT->pos.Set(m_worldWidth / 2, 95);
	borderT->normal = Rotation * Vector3(1, 0, 0);
	borderT->scale.Set(20, 180, 0);

	GameObject *borderL = FetchGO();
	borderL->type = GameObject::GO_BORDER;
	borderL->pos.Set(m_worldWidth / 2 - 40, m_worldHeight / 2);
	borderL->normal.Set(1, 0, 0);
	borderL->scale.Set(20, 100, 0);

	GameObject *borderR = FetchGO();
	borderR->type = GameObject::GO_BORDER;
	borderR->pos.Set(m_worldWidth / 2 + 40, m_worldHeight / 2);
	borderR->normal.Set(1, 0, 0);
	borderR->scale.Set(20, 100, 0);

	m_bouncer = FetchGO();
	/*Mtx44 Rotation;
	Rotation.SetToRotation(90, 0, 0, 1);*/
	//baGuaRotation * (baGuaCentre + baGuaOffset);
	m_bouncer->pos.Set(m_worldWidth / 2, 10);
	m_bouncer->normal = Rotation * Vector3(1, 0, 0);
	m_bouncer->type = GameObject::GO_BOUNCEWALL;
	m_bouncer->scale.Set(2,15,1);
	m_bouncer->mass = 10;

	m_pillarR = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	m_pillarR->pos = (m_bouncer->pos + Vector3(m_bouncer->scale.y * 0.5 * sin(Math::DegreeToRadian(-90)), m_bouncer->scale.y * 0.5 * cos(Math::DegreeToRadian(90)), 0));
	m_pillarR->type = GameObject::GO_BOUNCEPILLAR;
	m_pillarR->scale.Set(1, 1, 1);
	m_pillarR->mass = m_bouncer->mass;

	m_pillarL = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	m_pillarL->pos = (m_bouncer->pos - Vector3(m_bouncer->scale.y * 0.5 * sin(Math::DegreeToRadian(-90)), m_bouncer->scale.y * 0.5 * cos(Math::DegreeToRadian(90)), 0));
	m_pillarL->type = GameObject::GO_BOUNCEPILLAR;
	m_pillarL->scale.Set(1, 1, 1);
	m_pillarL->mass = m_bouncer->mass;
}

GameObject* SceneAssignment2::FetchGO()
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

void SceneAssignment2::CreatePillarGO(Vector3 wallScale, float rAngle, Vector3 centerPos)
{
	GameObject* wall = FetchGO();
	Mtx44 Rotation;
	Rotation.SetToRotation(rAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + baGuaOffset);
	wall->pos = centerPos;
	wall->normal = Rotation * Vector3(1, 0, 0);
	wall->type = GameObject::GO_WALL;
	wall->scale = wallScale;

	wall = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	wall->pos = (centerPos + Vector3(wallScale.y * 0.5 * sin(Math::DegreeToRadian(-rAngle)), wallScale.y * 0.5 * cos(Math::DegreeToRadian(rAngle)), 0));
	wall->type = GameObject::GO_PILLAR;
	wall->scale.Set(wallScale.x *0.5, wallScale.x * 0.5, 1);

	wall = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	wall->pos = (centerPos - Vector3(wallScale.y * 0.5 * sin(Math::DegreeToRadian(-rAngle)), wallScale.y * 0.5 * cos(Math::DegreeToRadian(rAngle)), 0));
	wall->type = GameObject::GO_PILLAR;
	wall->scale.Set(wallScale.x *0.5, wallScale.x * 0.5, 1);
}

bool SceneAssignment2::CheckCollision(GameObject *go1, GameObject *go2, float dt)
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
	case GameObject::GO_BORDER:
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
	case GameObject::GO_BOUNCEWALL:
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
	case GameObject::GO_PILLAR:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;
		float combinedRadius = r1 + r2;

		Vector3 u = go1->vel;

		return (p2 - p1).LengthSquared() < combinedRadius * combinedRadius && (p2 - p1).Dot(u) > 0;
	}
	case GameObject::GO_BOUNCEPILLAR:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;
		float combinedRadius = r1 + r2;

		Vector3 u = go1->vel;

		return (p2 - p1).LengthSquared() < combinedRadius * combinedRadius && (p2 - p1).Dot(u) > 0;
	}
	case GameObject::GO_POWERUP1:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;
		float combinedRadius = r1 + r2;

		Vector3 u = go1->vel;

		return (p2 - p1).LengthSquared() < combinedRadius * combinedRadius && (p2 - p1).Dot(u) > 0;
	}
	}
}

void SceneAssignment2::CollisionResponse(GameObject *go1, GameObject *go2)
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
		score += 10;
		break;
	}
	case GameObject::GO_BORDER:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->normal;
		Vector3 uN = u.Dot(N) * N;
		go1->vel = u - 2 * uN;
		break;
	}
	case GameObject::GO_BOUNCEWALL:
	{
		Vector3 u = go1->vel;
		Vector3 N = go2->normal;
		Vector3 uN = u.Dot(N) * N;
		go1->vel = u - 2 * uN;
		break;
	}
	case GameObject::GO_PILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 N = (go2->pos - go1->pos).Normalized();
		go1->vel = u - 2 * u.Dot(N) * N;
		go2->active = false;
		m_objectCount -= 1;
		obstacleCount -= 1;
		score += 10;
		break;
	}
	case GameObject::GO_BOUNCEPILLAR:
	{
		Vector3 u = go1->vel;
		Vector3 N = (go2->pos - go1->pos).Normalized();
		go1->vel = u - 2 * u.Dot(N) * N;
		break;
	}
	case GameObject::GO_POWERUP1:
	{
		power1 = true;
		powerCount1 -= 1;
		go2->active = false;
	}
	}
}

void SceneAssignment2::Update(double dt)
{
	SceneBase::Update(dt);
	if (ballCount >= 3)
		ballSpawn = false;
	if (ballCount < 3)
	{
		ballSpawn = true;
	}
	if (score < 0)
		score = 0;

	if (Application::IsKeyPressed('R'))
	{
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active && go->type == GameObject::GO_BALL)
			{
				go->active = false;
				--m_objectCount;
				--ballCount;
			}
		}
	}

	if (gameState == 0)
	{
		if (Application::IsKeyPressed('W') && selectY <= 34.5)
		{
			selectY += 0.25;
		}
		if (Application::IsKeyPressed('S') && selectY >= 24.5)
		{
			selectY -= 0.25;
		}
		if (Application::IsKeyPressed('E') && selectY <= 27 && selectY >= 24)
		{
			gameState = 1;
		}
		if (Application::IsKeyPressed('E') && selectY <= 35 && selectY >= 33)
		{
			gameState = 2;
		}
		static bool bQState = false;
		if (!bQState && Application::IsKeyPressed('Q') && !easyMode)
		{
			bQState = true;
			easyMode = true;
		}
		else if (bQState && !Application::IsKeyPressed('Q'))
		{
			bQState = false;
		}
		if (!bQState && Application::IsKeyPressed('Q') && easyMode)
		{
			bQState = true;
			easyMode = false;
		}
		else if (bQState && !Application::IsKeyPressed('Q'))
		{
			bQState = false;
		}
		if (easyMode)
			lives = 9;
		else
			lives = 3;
	}
	if (gameState == 1)
	{
		if (Application::IsKeyPressed('F'))
		{
			gameState = 0;
			selectY = 33.3;
		}
		if (easyMode)
			lives = 9;
		else
			lives = 3;
	}
	if (gameState == 2)
	{
		// Bouncer
		Vector3 Acceleration(0, 0, 0);
		Acceleration = m_force * (5 / m_bouncer->mass);

		m_bouncer->vel += Acceleration * dt;
		m_pillarL->vel = m_pillarR->vel = m_bouncer->vel;
		m_bouncer->pos += m_bouncer->vel *dt;
		m_pillarL->pos += m_pillarL->vel *dt;
		m_pillarR->pos += m_pillarR->vel *dt;

		if (m_bouncer->vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
		{
			m_bouncer->vel.Normalize() *= MAX_SPEED;
		}

		m_force.SetZero();

		if (Application::IsKeyPressed('A'))
		{
			//m_force.Set(-100, 0, 0);
			m_force.x += -30;
			if (m_bouncer->pos.x < 67.7)
				m_force.x = 0;
		}
		if (Application::IsKeyPressed('D'))
		{
			//m_force.Set(100, 0, 0);
			m_force.x += 30;
			if (m_bouncer->pos.x > 110)
				m_force.x = 0;
		}
		if (m_bouncer->pos.x < 67.7 || m_bouncer->pos.x > 110)
		{
			m_bouncer->vel = 0;
			m_pillarL->vel = m_pillarR->vel = m_bouncer->vel;
		}
		if (m_bouncer->vel.y < 0 || m_bouncer->vel.y >0)
			m_bouncer->vel.y = 0;
		if (m_bouncer->pos.y < 10 || m_bouncer->pos.y >10)
			m_pillarL->pos.y = m_pillarR->pos.y = m_bouncer->pos.y = 10;

		// Power-Up 1
		if (powerCount1 < 1 && !power1)
		powerTimer1 += dt;
		if (powerTimer1 > 5.f && !power1)
		{
			int Counter = 0;
			int MaxSpawn = 1;
			powerTimer1 = 0.f;
			//Exercise 9: spawn obstacles
			for (; Counter < MaxSpawn; ++Counter)
			{
				GameObject* Go = FetchGO();
				Go->type = GameObject::GO_POWERUP1;
				Go->pos.Set(Math::RandFloatMinMax(m_worldWidth / 2 - 25.f, m_worldWidth / 2 + 25.f), Math::RandFloatMinMax(20.f, m_worldHeight - 40.f), 0);
				Go->scale.Set(2, 2, 2);
				powerCount1 += 1;
				m_objectCount += 1;
			}
		}
		if (power1)
		{
			powerDuration1 += dt;
			m_pillarL->mass = m_pillarR->mass = m_bouncer->mass = 3;
			if (powerDuration1 > 10.f)
			{
				m_pillarL->mass = m_pillarR->mass = m_bouncer->mass = 10;
				powerDuration1 = 0;
				power1 = false;
			}
		}

		// Ball
		m_ghost->pos += m_bouncer->vel * dt;
		static bool bSpaceBarState = false;
		if (!bSpaceBarState && Application::IsKeyPressed(' ') && ballSpawn)
		{
			bSpaceBarState = true;
			std::cout << "SPACEBAR DOWN" << std::endl;

			//Exercise 10: spawn ghost ball
			if (m_ghost)
			{
				m_ghost->active = true;
				m_ghost->scale.Set(1, 1, 1);
				m_ghost->pos.Set(m_bouncer->pos.x, m_bouncer->pos.y + 2);
			}
		}
		else if (bSpaceBarState && !Application::IsKeyPressed(' '))
		{
			bSpaceBarState = false;
			std::cout << "SPACEBAR UP" << std::endl;

			//Exercise 6: spawn small GO_BALL
			GameObject * go = FetchGO();
			go->active = true;
			go->type = GameObject::GO_BALL;
			go->pos.Set(m_ghost->pos.x, m_ghost->pos.y, 0);
			go->vel = Vector3(m_bouncer->vel.x, 30, 0);
			go->scale.Set(1, 1, 1);
			m_ghost->active = false;
			ballCount += 1;
		}
	}
	if (gameState == 3)
	{
		if (highScore < score)
			highScore = score;
		if (Application::IsKeyPressed('F'))
		{
			Init();
		}
	}

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go1 = (GameObject *)*it;
		if (lives <= 0 || obstacleCount <= 0)
		{
			if (go1->active)
				go1->active = false;
			m_pillarL->vel = m_pillarR->vel = m_bouncer->vel = Vector3(0, 0, 0);
			gameState = 3;
		}
		//Physics Simulation Section
		if (go1->active)
		{
			if (go1->type == GameObject::GO_WALL || go1->type == GameObject::GO_PILLAR)
			{
				if (go1->vel != Vector3(0, 0, 0))
					go1->vel = Vector3(0, 0, 0);
			}
			if (go1->type == GameObject::GO_BALL)
			{
				go1->pos += go1->vel * dt;
				float radius = go1->scale.x;
				//Exercise 7: handle out of bound game objects
				if ((go1->pos.x > m_worldWidth - radius && go1->vel.x > 0) || (go1->pos.x < radius && go1->vel.x < 0))
					go1->vel.x = -go1->vel.x;

				if (go1->pos.y > m_worldHeight - radius && go1->vel.y > 0)
					go1->vel.y = -go1->vel.y;

				if (go1->pos.y < radius && go1->vel.y < 0)
				{
					go1->active = false;
					m_objectCount -= 1;
					ballCount -= 1;
					lives -= 1;
					score -= 5;
				}
			}

			//Exercise 8a: handle collision between objects
			for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject *go2 = (GameObject *)*it2;
				if (!go2->active)
					continue;
				/*if (go2->type != GameObject::GO_BALL)
				continue;*/

				GameObject *goA = go1, *goB = go2;
				if (go1->type != GameObject::GO_BALL)
				{
					if (go2->type != GameObject::GO_BALL)
						continue;
					goA = go2;
					goB = go1;
				}
				if (CheckCollision(go1, go2, dt))
				{
					CollisionResponse(go1, go2);
					for (std::vector<GameObject *>::iterator it3 = m_goList.begin(); it3 != m_goList.end(); ++it3)
					{
						GameObject *go3 = (GameObject *)*it3;
						if (go2->type == GameObject::GO_WALL)
						{
							if (go3->type == GameObject::GO_PILLAR)
							{
								Vector3 w0 = go2->pos;
								Vector3 b1 = go3->pos;
								Vector3 N = go2->normal;
								float r = go3->scale.x;
								float h = go2->scale.x;
								float l = go2->scale.y;

								Vector3 NP(-N.y, N.x);

								Vector3 RV = go3->vel;
								Vector3 RD = w0 - b1;

								if (RD.Dot(N) < 0)
									N = -N;

								if (abs(RD.Dot(N)) < r + h / 2 && abs(RD.Dot(NP)) < l)
								{
									go3->active = false;
									m_objectCount -= 1;
								}
							}
						}
						if (go2->type == GameObject::GO_PILLAR)
						{
							for (std::vector<GameObject *>::iterator it4 = m_goList.begin(); it4 != m_goList.end(); ++it4)
							{
								GameObject *go4 = (GameObject *)*it4;
								if (go4->type == GameObject::GO_PILLAR)
								{
									if (go3->type == GameObject::GO_WALL)
									{
										Vector3 w0 = go3->pos;
										Vector3 b1 = go2->pos;
										Vector3 b2 = go4->pos;
										Vector3 N = go3->normal;
										float r1 = go2->scale.x;
										float r2 = go4->scale.x;
										float h = go3->scale.x;
										float l = go3->scale.y;

										Vector3 NP(-N.y, N.x);

										Vector3 RV = go2->vel;
										Vector3 RD1 = w0 - b1;
										Vector3 RD2 = w0 - b2;

										if (RD1.Dot(N) < 0 || RD2.Dot(N) < 0)
											N = -N;

										if (abs(RD1.Dot(N)) < r1 + h / 2 && abs(RD2.Dot(N)) < r2 + h / 2 && abs(RD1.Dot(NP)) < l && abs(RD2.Dot(NP)) < l)
										{
											go4->active = false;
											go3->active = false;
											m_objectCount -= 1;
										}
									}
								}
							}
						}
					}
					break;
				}
			}
		}
	}
}


void SceneAssignment2::RenderGO(GameObject *go)
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

		modelStack.PushMatrix();
		Vector3 pillarR = (go->pos + Vector3(go->scale.y * 0.5 * sin(Math::DegreeToRadian(-angle)), go->scale.y * 0.5 * cos(Math::DegreeToRadian(angle)), 0));
		modelStack.Translate(pillarR.x, pillarR.y, pillarR.z);
		modelStack.Scale(go->scale.x * 0.5, go->scale.x * 0.5, 1);
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		Vector3 pillarL = (go->pos - Vector3(go->scale.y * 0.5 * sin(Math::DegreeToRadian(-angle)), go->scale.y * 0.5 * cos(Math::DegreeToRadian(angle)), 0));
		modelStack.Translate(pillarL.x, pillarL.y, pillarL.z);
		modelStack.Scale(go->scale.x * 0.5, go->scale.x * 0.5, 1);
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_BORDER:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_BORDER], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_BOUNCEWALL:
	{
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		float angle = Math::RadianToDegree(atan2(go->normal.y, go->normal.x));
		modelStack.Rotate(angle, 0, 0, 1);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_WALL], false);
		modelStack.PopMatrix();
		break;
	}
	//case GameObject::GO_PILLAR:
	//{	//Exercise 4: render a sphere using scale and pos
	//	modelStack.PushMatrix();
	//	modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
	//	modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
	//	RenderMesh(meshList[GEO_PILLAR], false);
	//	modelStack.PopMatrix();
	//	break;
	//}
	case GameObject::GO_BOUNCEPILLAR:
	{	//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();
		break;
	}
	case GameObject::GO_POWERUP1:
	{	//Exercise 4: render a sphere using scale and pos
		modelStack.PushMatrix();
		modelStack.Translate(go->pos.x, go->pos.y, go->pos.z);
		modelStack.Scale(go->scale.x, go->scale.y, go->scale.z);
		RenderMesh(meshList[GEO_POWER1], false);
		modelStack.PopMatrix();
		break;
	}
	}
}

void SceneAssignment2::Render()
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

	switch (gameState)
	{
	case 0:
		{
			modelStack.PushMatrix();
			modelStack.Translate(selectX, selectY, 0);
			modelStack.Scale(1.5, 1.5, 1);
			RenderMesh(meshList[GEO_BALL], false);
			modelStack.PopMatrix();

			std::ostringstream ss;
			ss.precision(1);
			ss << "<PHYSICS ASSIGNMENT 2>";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 5, 3, 40);

			ss.str("");
			ss.precision(1);
			ss << "Start";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 37.2, 19);

			ss.str("");
			ss.precision(1);
			ss << "How To Play";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 37.2, 14);

			if (!easyMode)
			{
				ss.str("");
				ss.precision(1);
				ss << "Press <Q> to toggle mode: Normal";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 25, 9);
			}
			if (easyMode)
			{
				ss.str("");
				ss.precision(1);
				ss << "Press <Q> to toggle mode: Easy";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 25, 9);
			}

			ss.str("");
			ss.precision(1);
			ss << "Press <E> to select an option";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 25, 6);

			ss.str("");
			ss.precision(1);
			ss << "Press <Esc> to exit the game";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 25, 3);
			break;
		}
	case 1:
		{
			std::ostringstream ss;
			ss.precision(1);
			ss << "Use <A> and <D> to move left or right respectively";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 55);

			ss.str("");
			ss.precision(1);
			ss << "Press <Spacebar> to shoot a ball";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 48);

			ss.str("");
			ss.precision(1);
			ss << "Only 3 balls may be active at the same time";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 41);

			ss.str("");
			ss.precision(1);
			ss << "Destroy all walls to complete the level";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 34);

			ss.str("");
			ss.precision(1);
			ss << "Activate power ups by hitting purple balls which spawn";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 27);

			ss.str("");
			ss.precision(1);
			ss << "in a random location every few seconds";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 25);

			/*ss.str("");
			ss.precision(1);
			ss << "Missiles are homing and kill enemy ships in one hit";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 20);

			ss.str("");
			ss.precision(1);
			ss << "Enemy ships take damage when hit by bullets or colliding";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 13);

			ss.str("");
			ss.precision(1);
			ss << "with asteroids";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 11.5);

			ss.str("");
			ss.precision(1);
			ss << "Destroy all enemy ships to proceed to next wave";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 6);*/

			ss.str("");
			ss.precision(1);
			ss << "Press <F> to return to the main menu";
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 2, 0);
			break;
		}
	case 2:
		{
			if (m_ghost->active)
			{
				RenderGO(m_ghost);
			}

			for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
			{
				GameObject *go = (GameObject *)*it;
				if (go->active)
				{
					RenderGO(go);
				}
			}
			
			//On screen text

			//Exercise 5: Render m_objectCount
			std::ostringstream ss;
			if (power1)
			{
				ss.precision(2);
				ss << "Mass reduced for: " << 10 - powerDuration1 << "s";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 18, 55);
			}
			else
			{
				ss.precision(10);
				ss << "Power-Up inactive";
				RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 23, 55);
			}

			ss.str("");
			ss.precision(10);
			ss << "Score: " << score;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 3);

			ss.str("");
			ss.precision(10);
			ss << "Lives: " << lives;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 1);

			ss.str("");
			ss.precision(5);
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 0, 57);
			break;
		}
	case 3:
		std::ostringstream ss;
		ss.precision(1);
		ss << "GAME OVER";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 5, 26, 35);

		ss.str("");
		ss.precision(1);
		ss << "High Score: " << highScore;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 13.5, 19.5);

		ss.str("");
		ss.precision(1);
		ss << "Your Score: " << score;
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 13.5, 16.5);

		ss.str("");
		ss.precision(1);
		ss << "Press <F> to return to the main menu";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 2, 13.5, 13.5);
	}
}

void SceneAssignment2::Exit()
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
