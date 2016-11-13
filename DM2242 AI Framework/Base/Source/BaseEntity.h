#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include "Vector3.h"
 
class BaseEntity
{
public:
    BaseEntity();
    virtual ~BaseEntity();
    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual int GetHP() = 0;
    virtual void SetHP(int hp) = 0;
    virtual bool GetDead();
    virtual void SetDead(bool dead) = 0;
    virtual void Delete() = 0;
    virtual void SetPosition(Vector3 pos) = 0;
    virtual Vector3 GetPoition();
protected:
    int HP;
    bool Dead;
    Vector3 Position;
};

#endif