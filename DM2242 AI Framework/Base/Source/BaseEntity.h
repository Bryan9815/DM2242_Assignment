#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H
 
class BaseEntity
{
public:
    BaseEntity();
    virtual ~BaseEntity();
    virtual void Init();
    virtual void update();
    virtual void GetHP();
    virtual void SetHP(int hp);
    virtual bool GetDead();
    virtual bool SetDead(bool dead);
    virtual void Delete();
protected:
    int HP;
    bool Dead;

};

#endif