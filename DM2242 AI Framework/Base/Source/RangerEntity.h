#ifndef RANGER_ENTITY_H
#define RANGER_ENTITY_H
#include "BaseEntity.h"
class RangerEntity : public BaseEntity
{
public:
    RangerEntity();
    ~RangerEntity();

    void Init();
    void Update();
    int GetHP();
    void SetHP(int hp);
    bool GetDead();
    void SetDead(bool dead);
    void Delete();
    void SetPosition(Vector3 pos);
    Vector3 GetPoition();
private:
    bool DeadAlly;

};

#endif
