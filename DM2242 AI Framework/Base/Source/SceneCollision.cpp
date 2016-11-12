#include "SceneCollision.h"
#include "GL\glew.h"
#include "Application.h"
#include <sstream>

SceneCollision::SceneCollision()
{
}

SceneCollision::~SceneCollision()
{
}

void SceneCollision::Init()
{
	SceneBase::Init();

	//Physics code here
	m_speed = 1.f;

	Math::InitRNG();

	//Exercise 1: initialize m_objectCount
	m_objectCount = 0;
	m_ghost = new GameObject(GameObject::GO_BALL);

	Vector3 Center(90, 50, 0);
	Vector3 Offset(35, 0, 0);
	Mtx44 Rotation;
	for (int i = 0; i < 8; i++)
	{
		GameObject *wall = FetchGO();
		Rotation.SetToRotation(i * 45.0f, 0, 0, 1);
		wall->type = GameObject::GO_WALL;
		wall->pos = (Rotation * Offset + Center);
		wall->normal = Rotation * Vector3(1, 0, 0);
		wall->active = true;
		wall->scale.Set(4, 40, 1);
	}

	GameObject *wall = FetchGO();
	wall->type = GameObject::GO_WALL;
	Rotation.SetToRotation(90.f, 0, 0, 1);
	wall->pos = Center;
	wall->normal = Rotation * Vector3(1, 0, 0);
	wall->scale.Set(4, 40, 1);
	wall->active = true;

	GameObject *pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->pos = Center + Vector3(wall->scale.y / 2, 0, 0);
	pillar->scale.Set(2, 2, 1);
	pillar->active = true;

	pillar = FetchGO();
	pillar->type = GameObject::GO_PILLAR;
	pillar->pos = Center + Vector3(-wall->scale.y / 2, 0, 0);
	pillar->scale.Set(2, 2, 1);
	pillar->active = true;

	/*GameObject *gWell = FetchGO();
	gWell->type = GameObject::GO_POWERUP1;
	gWell->pos.Set(50, 20, 0);
	gWell->active = true;
	gWell->scale.Set(3, 3, 3);*/
}

GameObject* SceneCollision::FetchGO()
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

void SceneCollision::CreatePillarGO(Vector3 wallScale, Vector3 ballScale, float rAngle, Vector3 centerPos)
{
	GameObject* wall = FetchGO();
	Mtx44 Rotation;
	Rotation.SetToRotation(rAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + baGuaOffset);
	wall->pos = centerPos;
	wall->normal = Rotation * Vector3(1, 0, 0);
	wall->type = GameObject::GO_WALL;
	wall->active = true;
	wall->scale = wallScale;

	wall = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	wall->pos = (centerPos + Vector3(wallScale.y * 0.5 * sin(Math::DegreeToRadian(-rAngle)), wallScale.y * 0.5 * cos(Math::DegreeToRadian(rAngle)), 0));
	wall->type = GameObject::GO_PILLAR;
	wall->active = true;
	wall->scale = ballScale;

	wall = FetchGO();
	//baGuaRotation.SetToRotation(RAngle, 0, 0, 1);
	//baGuaRotation * (baGuaCentre + Vector3(10,0,0));
	wall->pos = (centerPos - Vector3(wallScale.y * 0.5 * sin(Math::DegreeToRadian(-rAngle)), wallScale.y * 0.5 * cos(Math::DegreeToRadian(rAngle)), 0));
	wall->type = GameObject::GO_PILLAR;
	wall->active = true;
	wall->scale = ballScale;
}

bool SceneCollision::CheckCollision(GameObject *go1, GameObject *go2, float dt)
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
		Vector3 w0 = go2->pos;
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
	case GameObject::GO_POWERUP1:
	{
		Vector3 p1 = go1->pos;
		Vector3 p2 = go2->pos;
		float r1 = go1->scale.x;
		float r2 = go2->scale.x;
		float combinedRadius = r1 + r2;

		return (p2 - p1).LengthSquared() < combinedRadius * combinedRadius;
	}
	}
	return false;
}

float SceneCollision::CheckCollision2(GameObject *go1, GameObject *go2)
{
	switch (go2->type)
	{
	case GameObject::GO_BALL:
	{
		Vector3 rel = go1->vel - go2->vel;
		Vector3 dir = go1->pos - go2->pos;
		float r = go1->scale.x + go2->scale.x;
		float dot = rel.Dot(dir);

		if (dot > 0)
			return -1;

		float a = rel.LengthSquared();
		float b = 2 * rel.Dot(dir);
		float c = dir.LengthSquared() - r * r;
		float d = b * b - 4 * a * c;

		if (d < 0)
			return -1;

		float t = (-b - sqrt(d)) / (2 * a);
		if (t < 0)
			t = (-b + sqrt(d)) / (2 * a);
		return t;

		return false;
		break;
	}
	case GameObject::GO_WALL:
	{
		Vector3 w0 = go2->pos;
		Vector3 b1 = go1->pos;
		Vector3 N = go2->normal;
		float r = go1->scale.x;
		float h = go2->scale.x;
		float l = go2->scale.y;

		Vector3 rv = go1->vel;
		Vector3 rd = w0 - b1;

		if (rd.Dot(N) < 0)
			N = -N;

		if (rv.Dot(N) < 0)
			return -1;

		w0 += -(r + h * 0.5) * N;
		float distance = (w0 - b1).Dot(N);
		float speed = rv.Dot(N);
		float th = distance / speed;

		Vector3 NP(-N.y, N.x);

		Vector3 w1 = w0 + (l * 0.5) * NP;
		Vector3 w2 = w0 - (l * 0.5) * NP;

		Vector3 bh = b1 + rv * th;

		if ((w1 - bh).Dot(w2 - bh) > 0)
			return -1;

		return th;
	}
	}
}

void SceneCollision::CollisionResponse(GameObject *go1, GameObject *go2)
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
		break;
	}
	case GameObject::GO_POWERUP1:
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
		break;
	}
	}
}

void SceneCollision::Update(double dt)
{
	SceneBase::Update(dt);
	//Keyboard Section
	if (Application::IsKeyPressed('9'))
	{
		m_speed = Math::Max(0.f, m_speed - 0.1f);
	}
	if (Application::IsKeyPressed('0'))
	{
		m_speed += 0.1f;
	}

	if (Application::IsKeyPressed('R'))
	{
		for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
		{
			GameObject *go = (GameObject *)*it;
			if (go->active && go->type == GameObject::GO_BALL)
			{
				go->active = false;
				--m_objectCount;
			}
		}
	}

	//Mouse Section
	static bool bLButtonState = false;
	if (!bLButtonState && Application::IsMousePressed(0))
	{
		bLButtonState = true;
		std::cout << "LBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		//Exercise 10: spawn ghost ball
		if (m_ghost)
		{
			m_ghost->active = true;
			m_ghost->scale.Set(1, 1, 1);
			//m_ghost->type = GameObject::GO_BALL;
			//m_ghost->vel.SetZero();
			//m_ghost->scale.Set(1, 1, 1);

			double x, y;
			Application::GetCursorPos(&x, &y);
			int h = Application::GetWindowHeight();
			int w = Application::GetWindowWidth();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			m_ghost->pos.Set(worldX, worldY);
		}
	}
	else if (bLButtonState && !Application::IsMousePressed(0))
	{
		bLButtonState = false;
		std::cout << "LBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject * go = FetchGO();

		//Exercise 10: replace Exercise 4 code and use ghost to determine ball velocity
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		go->active = true;
		go->type = GameObject::GO_BALL;
		go->pos.Set(m_ghost->pos.x, m_ghost->pos.y, 0);
		go->vel = m_ghost->pos - Vector3(worldX, worldY, 0);
		//go->vel *= 5.f;
		m_ghost->active = false;
		m_ghost->vel = go->vel;

	}
	static bool bRButtonState = false;
	if (!bRButtonState && Application::IsMousePressed(1))
	{
		bRButtonState = true;
		std::cout << "RBUTTON DOWN" << std::endl;

		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();

		//Exercise 10: spawn ghost ball
		if (m_ghost)
		{
			m_ghost->active = true;
			m_ghost->scale.Set(3, 3, 3);
			//m_ghost->type = GameObject::GO_BALL;
			//m_ghost->vel.SetZero();
			//m_ghost->scale.Set(1, 1, 1);

			double x, y;
			Application::GetCursorPos(&x, &y);
			int h = Application::GetWindowHeight();
			int w = Application::GetWindowWidth();
			float worldX = x * m_worldWidth / w;
			float worldY = (h - y) * m_worldHeight / h;
			m_ghost->pos.Set(worldX, worldY);
		}
	}
	else if (bRButtonState && !Application::IsMousePressed(1))
	{
		bRButtonState = false;
		std::cout << "RBUTTON UP" << std::endl;

		//Exercise 6: spawn small GO_BALL
		GameObject * go = FetchGO();

		//Exercise 10: replace Exercise 4 code and use ghost to determine ball velocity
		double x, y;
		Application::GetCursorPos(&x, &y);
		int w = Application::GetWindowWidth();
		int h = Application::GetWindowHeight();
		float worldX = x * m_worldWidth / w;
		float worldY = (h - y) * m_worldHeight / h;
		go->active = true;
		go->type = GameObject::GO_BALL;
		go->pos.Set(m_ghost->pos.x, m_ghost->pos.y, 0);
		go->vel = m_ghost->pos - Vector3(worldX, worldY, 0);
		go->scale.Set(3, 3, 3);
		go->mass = 3 * 3 * 3;
		//go->vel *= 5.f;
		m_ghost->active = false;
		m_ghost->vel = go->vel;
		//Exercise 10: spawn large GO_BALL
	}

	//Physics Simulation Section

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			if (go->type == GameObject::GO_BALL)
			{
				go->pos += go->vel * dt;
				float radius = go->scale.x;
				//Exercise 7: handle out of bound game objects
				if ((go->pos.x > m_worldWidth - radius && go->vel.x > 0) || (go->pos.x < radius && go->vel.x < 0))
					go->vel.x = -go->vel.x;

				if ((go->pos.y > m_worldHeight - radius && go->vel.y > 0) || (go->pos.y < radius && go->vel.y < 0))
					go->vel.y = -go->vel.y;

				if (go->pos.y < -20 || go->pos.x < -20 || go->pos.y > m_worldHeight + 20 || go->pos.x > m_worldWidth + 20)
				{
					go->active = false;
					m_objectCount -= 1;
				}
			}

			//Exercise 8a: handle collision between GO_BALL and GO_BALL using velocity swap
			for (std::vector<GameObject *>::iterator it2 = m_goList.begin(); it2 != m_goList.end(); ++it2)
			{
				GameObject *other = (GameObject *)*it2;
				if (!other->active)
					continue;
				/*if (other->type != GameObject::GO_BALL)
				continue;*/

				GameObject *goA = go, *goB = other;
				if (go->type != GameObject::GO_BALL)
				{
					if (other->type != GameObject::GO_BALL)
						continue;
					goA = other;
					goB = go;
				}
				if (CheckCollision(go, other, dt))
					/*if (CheckCollision2(go, other))*/
				{
					CollisionResponse(go, other);
					break;
				}
			}
		}
	}
}


void SceneCollision::RenderGO(GameObject *go)
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
		break;
	}
	case GameObject::GO_PILLAR:
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
		RenderMesh(meshList[GEO_PILLAR], false);
		modelStack.PopMatrix();
		break;
	}
	}
}

void SceneCollision::Render()
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

	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->active)
		{
			RenderGO(go);
		}
	}
	if (m_ghost->active)
	{
		RenderGO(m_ghost);
	}

	//On screen text

	//Exercise 5: Render m_objectCount
	std::ostringstream ss;
	ss.precision(2);
	ss << "Object Count: " << m_objectCount;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 21);
	//Exercise 8c: Render initial and final momentum

	ss.str("");
	ss.precision(3);
	ss << "Speed: " << m_speed;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	ss.str("");
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 3);

	RenderTextOnScreen(meshList[GEO_TEXT], "Collision", Color(0, 1, 0), 3, 0, 0);
}

void SceneCollision::Exit()
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
