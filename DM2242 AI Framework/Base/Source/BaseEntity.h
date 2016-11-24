#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "Vector3.h"
#include "StateMachine.h"
class BaseEntity
{
public:
    BaseEntity();
    virtual ~BaseEntity();
	virtual void Init(){};
	virtual void Update(double dt){};
    virtual int GetHP();
    virtual void SetHP(int hp);
	virtual unsigned int GetAggro();
	virtual void SetAggro(unsigned int aggro);
    virtual bool GetDead();
    virtual void SetDead(bool dead);
    virtual void Delete() = 0;
    virtual void SetPosition(Vector3 pos);
    virtual Vector3 GetPosition();
    virtual void SetName(std::string Name);
    virtual std::string GetName();
    virtual float GetScale();
protected:
    int HP = 100;
	unsigned int Aggro = 0;
    bool Dead = false;
    Vector3 Position;
    std::string Name;
    float world_width;
    float world_height;
    float scale = 2.f;
	float ReviveRange = 2.0f;
    Vector3 MovementDirVec;
};

#endif