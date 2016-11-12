#include "SceneAsteroid.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneAsteroid::SceneAsteroid()
{
}

SceneAsteroid::~SceneAsteroid()
{
}

void SceneAsteroid::Init()
{
	SceneBase::Init();

	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

	//Physics code here
	m_speed = 1.f;
	
	Math::InitRNG();

	//Exercise 2a: Construct 100 GameObject with type GO_ASTEROID and add into m_goList
	for (int i = 0; i < 100; i++)
	{
		GameObject *newObj = new GameObject(GameObject::GO_ASTEROID);
		m_goList.push_back(newObj);
	}
	for (int i = 0; i < 100; i++)
	{
		GameObject *newObj = new GameObject(GameObject::GO_ESHIP);
		m_goList.push_back(newObj);
	}
	//Exercise 2b: Initialize m_lives and m_score
	m_lives = 1;
	m_score = 0;
	//Exercise 2c: Construct m_ship, set active, type, scale and pos
	m_ship = NULL;
	m_ship = new GameObject(GameObject::GO_SHIP);
	m_ship->scale.Set(1, 1, 1);
	m_ship->vel.SetZero();
	m_ship->pos.Set(m_worldWidth/2, m_worldHeight/2);
	m_ship->mass = 5;
	m_ship->active = true;
}

GameObject* SceneAsteroid::FetchGO()
{
	//Exercise 3a: Fetch a game object from m_goList and return it
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		if (!(*it)->active)
		{
			(*it)->active = true;
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
	return go;
}

void SceneAsteroid::Update(double dt)
{
	SceneBase::Update(dt);
	
	if(Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if(Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}
	m_force.SetZero();
	//Exercise 6: set m_force values based on WASD
	if(Application::IsKeyPressed('W'))
	{
		//m_force.Set(0, 100, 0); 
		m_force.y = 100;
	}
	if(Application::IsKeyPressed('A'))
	{
		//m_force.Set(-100, 0, 0);
		m_force.x = -100;
	}
	if(Application::IsKeyPressed('S'))
	{
		//m_force.Set(0, -100, 0);
		m_force.y = -100;
	}
	if(Application::IsKeyPressed('D'))
	{
		//m_force.Set(100, 0, 0);
		m_force.x = 100;
	}

	//Exercise 8: use 2 keys to increase and decrease mass of ship
	if (Application::IsKeyPressed('9'))
	{
		m_ship->mass = Math::Max(0.1f, m_ship->mass - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_ship->mass += 0.1f;
	}

	//Exercise 11: use a key to spawn some asteroids
	if (Application::IsKeyPressed('V'))
	{
		int Counter = 0;
		int MaxSpawn = 25;
		//Exercise 9: spawn obstacles
		for (; Counter < MaxSpawn; ++Counter)
		{
			GameObject* Go = FetchGO();
			Go->active = true;
			Go->type = GameObject::GO_ASTEROID;
			Go->pos.Set(Math::RandFloatMinMax(0.f, m_worldWidth), Math::RandFloatMinMax(0.f, m_worldHeight), 0);
			Go->vel.Set(Math::RandFloatMinMax(-5.f, 5.f), Math::RandFloatMinMax(-5.f, 5.f), 0);
			Go->scale.Set(1, 1, 1);
		}
	}
	FireDebounceTimer += dt;

	//Spawn Enemy Ships
	if (Application::IsKeyPressed('P'))
	{
		int Counter = 0;
		int MaxSpawn = 5;
		//Exercise 9: spawn obstacles
		for (; Counter < MaxSpawn; ++Counter)
		{
			GameObject* Go = FetchGO();
			Go->active = true;
			Go->type = GameObject::GO_ESHIP;
			Go->pos.Set(Math::RandFloatMinMax(0.f, m_worldWidth), Math::RandFloatMinMax(0.f, m_worldHeight), 0);
			Go->vel.Set(Math::RandFloatMinMax(-5.f, 5.f), Math::RandFloatMinMax(-5.f, 5.f), 0);
			Go->scale.Set(1, 1, 1);
		}
	}
	FireDebounceTimer += dt;

	//Exercise 14: use a key to spawn a bullet
	if (Application::IsKeyPressed(VK_SPACE) && FireDebounceTimer > 1.f/FireRate)
	{
			GameObject* Go = FetchGO();
			Go->active = true;
			Go->type = GameObject::GO_BULLET;
			Go->pos = m_ship->pos;
			Go->scale.Set(0.2f, 0.2f, 0.2f);
			FireDebounceTimer = 0.f;
			if (m_ship->vel.IsZero())
			{
				Go->vel = Vector3(0, 1, 0) * BULLET_SPEED;
			}
			else Go->vel = m_ship->vel.Normalized() * BULLET_SPEED;
	}
	if (Application::IsKeyPressed('M') && FireDebounceTimer > 1.f / FireRate)
	{
		GameObject* Go = FetchGO();
		Go->active = true;
		Go->type = GameObject::GO_MISSILE;
		Go->pos = m_ship->pos;
		Go->scale.Set(0.3f, 0.3f, 0.3f);
		FireDebounceTimer = 0.f;
	}
	//Exercise 15: limit the spawn rate of bullets

	//Mouse Section
	static bool bLButtonState = false;
	if(!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;
	}
	else if(bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;
	}
	static bool bRButtonState = false;
	if(!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;
	}
	else if(bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;
	}

	//Physics Simulation Section

	//Exercise 7: Update ship's velocity based on m_force
	//Vector3 Acceleration = Vector3(m_force.x / m_ship->mass, m_force.y / m_ship->mass, 0);
	Vector3 Acceleration(0, 0, 0);
	if (m_ship->mass > Math::EPSILON)
	{
		Acceleration = m_force * (1 / m_ship->mass);
	}
	m_ship->vel += Acceleration * dt;
	m_ship->pos += m_ship->vel *dt;

	if (m_ship->vel.LengthSquared() > MAX_SPEED * MAX_SPEED)
	{
		m_ship->vel.Normalize() *= MAX_SPEED;
	}

	//Exercise 9: wrap ship position if it leaves screen
	if (m_ship->pos.y > m_worldHeight) m_ship->pos.y -= m_worldHeight;
	if (m_ship->pos.y < 0) m_ship->pos.y += m_worldHeight;

	if (m_ship->pos.x >m_worldWidth) m_ship->pos.x -= m_worldWidth;
	if (m_ship->pos.x < 0) m_ship->pos.x += m_worldWidth;

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			Vector3 storedVel = go->vel;
			go->pos += (storedVel + go->vel) * 0.5f * dt;
			//Exercise 12: handle collision between GO_SHIP and GO_ASTEROID using simple distance-based check
			if (go->type == GameObject::GO_ASTEROID && go->active)
			{
				// Simple BC.
				float CombinedRadiusSquared = (m_ship->scale.x + go->scale.x) * (m_ship->scale.x + go->scale.x);
				float DistSquared = (m_ship->pos - go->pos).LengthSquared();
				if (DistSquared < CombinedRadiusSquared)
				{
					go->active = false;
					if (--m_lives <= 0)
					{
						m_score = 0;
						m_lives = 3;
					}
					else
					{
						--m_lives;
					}
					m_ship->pos.Set(m_worldWidth / 2, m_worldHeight / 2, 0);
					m_ship->vel.SetZero();
					break;
				}
			}
			//Exercise 13: asteroids should wrap around the screen like the ship
			if (go->type == GameObject::GO_ASTEROID){
				if (go->pos.y > m_worldHeight) go->pos.y -= m_worldHeight;
				if (go->pos.y < 0) go->pos.y += m_worldHeight;

				if (go->pos.x >m_worldWidth) go->pos.x -= m_worldWidth;
				if (go->pos.x < 0) go->pos.x += m_worldWidth;
			}
			
			//Exercise 16: unspawn bullets when they leave screen
			if (go->type == GameObject::GO_BULLET || go->type == GameObject::GO_MISSILE){
				if (go->pos.y > m_worldHeight) go->active = false;
				if (go->pos.y < 0) go->active = false;

				if (go->pos.x > m_worldWidth) go->active = false;
				if (go->pos.x < 0) go->active = false;
			}

			//Exercise 18: collision check between GO_BULLET and GO_ASTEROID
			if (go->type == GameObject::GO_BULLET)
			for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				if ((*it2)->type == GameObject::GO_ASTEROID && (*it2)->active)
				{
					// Simple BC.
					float CombinedRadiusSquared = (go->scale.x + (*it2)->scale.x) * (go->scale.x + (*it2)->scale.x);
					float DistSquared = ((*it2)->pos - go->pos).LengthSquared();
					if (DistSquared < CombinedRadiusSquared)
					{
						(*it2)->active = go->active = false;
						m_score += 2;
						break;
					}
				}
				if ((*it2)->type == GameObject::GO_ESHIP && (*it2)->active)
				{
					// Simple BC.
					float CombinedRadiusSquared = (go->scale.x + (*it2)->scale.x) * (go->scale.x + (*it2)->scale.x);
					float DistSquared = ((*it2)->pos - go->pos).LengthSquared();
					if (DistSquared < CombinedRadiusSquared)
					{
						(*it2)->active = go->active = false;
						m_score += 2;
						break;
					}
				}
			}
			else if (go->type == GameObject::GO_MISSILE)
			{
				GameObject* ClosestAsteroid = NULL;
				float ClosestDistanceSquared = FLT_MAX;
				for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					if ((*it2)->type == GameObject::GO_ASTEROID && (*it2)->active)
					{
						//Find the asteroid closest to the missile
						float LengthSquared = (go->pos - (*it2)->pos).LengthSquared();
						if (LengthSquared < ClosestDistanceSquared)
						{
							ClosestDistanceSquared = Math::Min(LengthSquared, ClosestDistanceSquared);
							ClosestAsteroid = (*it2);
						}
						float CombinedRadiusSquared = (go->scale.x + (*it2)->scale.x) * (go->scale.x + (*it2)->scale.x);
						if (LengthSquared < CombinedRadiusSquared)
						{
							(*it2)->active = go->active = false;
							m_score += 1;
							break;
						}
					}
					else if ((*it2)->type == GameObject::GO_ESHIP && (*it2)->active)
					{
						//Find the asteroid closest to the missile
						float LengthSquared = (go->pos - (*it2)->pos).LengthSquared();
						if (LengthSquared < ClosestDistanceSquared)
						{
							ClosestDistanceSquared = Math::Min(LengthSquared, ClosestDistanceSquared);
							ClosestAsteroid = (*it2);
						}
						float CombinedRadiusSquared = (go->scale.x + (*it2)->scale.x) * (go->scale.x + (*it2)->scale.x);
						if (LengthSquared < CombinedRadiusSquared)
						{
							(*it2)->active = go->active = false;
							m_score += 1;
							break;
						}
					}
				}
				if (ClosestAsteroid != NULL)
				{
					Vector3 Displacement = (ClosestAsteroid->pos - go->pos);
					if (!Displacement.IsZero()){
						go->vel = Displacement.Normalized() * MISSILE_SPEED;
					}
				}
			}
			else if (go->type == GameObject::GO_ESHIP)
			{
				for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
				{
					if ((*it2)->type == GameObject::GO_ASTEROID && (*it2)->active)
					{
						// Simple BC.
						float CombinedRadiusSquared = (go->scale.x + (*it2)->scale.x) * (go->scale.x + (*it2)->scale.x);
						float DistSquared = ((*it2)->pos - go->pos).LengthSquared();
						if (DistSquared < CombinedRadiusSquared)
						{
							(*it2)->active = go->active = false;
							m_score += 2;
							break;
						}
					}
				}
				//Find the Player Ship
				float LengthSquared = (go->pos - m_ship->pos).LengthSquared();
						
				float CombinedRadiusSquared = (go->scale.x + m_ship->scale.x) * (go->scale.x + m_ship->scale.x);
				if (LengthSquared < CombinedRadiusSquared)
				{
					go->active = false;
					m_score += 1;
					break;
				}
				if (m_ship != NULL)
				{
					Vector3 Displacement = (m_ship->pos - go->pos);
					if (!Displacement.IsZero()){
						go->vel = Displacement.Normalized() * eShip_Speed;
					}
				}
			}
		}
	}
}


void SceneAsteroid::RenderGO(GameObject *go)
{
	float RotateAngle;
	
	if (go->active)
	switch(go->type)
	{
	case GameObject::GO_SHIP:
		//Exercise 4a: render a sphere with radius 1
		modelStack.PushMatrix();
		modelStack.Translate((*go).pos.x, (*go).pos.y, (*go).pos.z + Increment);
		//Exercise 17a: render a ship texture or 3D ship model
		//Exercise 17b:	re-orientate the ship with velocity
		RotateAngle = Math::RadianToDegree(atan2(-go->vel.x, go->vel.y));
		modelStack.Rotate(RotateAngle, 0, 0, 1);
		modelStack.Scale((*go).scale.x, (*go).scale.y, (*go).scale.z);
		RenderMesh(meshList[GEO_SHIP], false);
		modelStack.PopMatrix();
		Increment += 0.01f;
		break;
	case GameObject::GO_ASTEROID:
		//Exercise 4b: render a cube with length 2
		modelStack.PushMatrix();
		modelStack.Translate((*go).pos.x, (*go).pos.y, (*go).pos.z + Increment);
		modelStack.Scale((*go).scale.x, (*go).scale.y, (*go).scale.z);
		RenderMesh(meshList[GEO_ASTEROID], false);
		modelStack.PopMatrix();
		Increment += 0.01f;
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate((*go).pos.x, (*go).pos.y, (*go).pos.z + Increment);
		modelStack.Scale((*go).scale.x, (*go).scale.y, (*go).scale.z);
		RenderMesh(meshList[GEO_BALL], false);
		modelStack.PopMatrix();
		Increment += 0.01f;
		break;
	case GameObject::GO_MISSILE:
		modelStack.PushMatrix();
		modelStack.Translate((*go).pos.x, (*go).pos.y, (*go).pos.z + Increment);
		modelStack.Scale((*go).scale.x, (*go).scale.y, (*go).scale.z);
		RenderMesh(meshList[GEO_MISSILE], false);
		modelStack.PopMatrix();
		Increment += 0.01f;
		break;
	case GameObject::GO_ESHIP:
		modelStack.PushMatrix();
		modelStack.Translate((*go).pos.x, (*go).pos.y, (*go).pos.z + Increment);
		modelStack.Scale((*go).scale.x, (*go).scale.y, (*go).scale.z);
		RenderMesh(meshList[GEO_ESHIP], false);
		modelStack.PopMatrix();
		Increment += 0.01f;
		break;
	}
}

void SceneAsteroid::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//Calculating aspect ratio
	m_worldHeight = 100.f;
	m_worldWidth = m_worldHeight * (float)Application::GetWindowWidth() / Application::GetWindowHeight();

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
	
	RenderMesh(meshList[GEO_AXES], false);

	for(std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if(go->active)
		{
			RenderGO(go);
		}
	}
	RenderGO(m_ship);
	Increment = 0;
	//On screen text

	//Exercise 5a: Render m_lives, m_score
	std::ostringstream ss;
	ss.precision(2);
	ss << "Lives: " << m_lives;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);

	ss.str("");
	ss.precision(2);
	ss << "Score: " << m_score;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 18);

	//Exercise 5b: Render position, velocity & mass of ship
	ss.str("");
	ss.precision(5);
	ss << "Ship Position: " << m_ship->pos;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 15);

	ss.str("");
	ss.precision(2);
	ss << "Ship Velocity: " << m_ship->vel;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 12);

	ss.str("");
	ss.precision(2);
	ss << "Ship Mass: " << m_ship->mass;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 9);

	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
	
	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);
	
	RenderTextOnScreen(meshList[GEO_TEXT], "Asteroid", Color(0, 1, 0), 3, 0, 0);
}

void SceneAsteroid::Exit()
{
	SceneBase::Exit();
	//Cleanup GameObjects
	while(m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
	if(m_ship)
	{
		delete m_ship;
		m_ship = NULL;
	}
}
