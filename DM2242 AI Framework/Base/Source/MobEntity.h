#ifndef MOB_ENTITY_H
#define MOB_ENTITY_H
#include "BaseEntity.h"
#include "StateMachine.h"
class MobEntity : public BaseEntity
{
public:
    MobEntity();
    ~MobEntity();

    void Init();
    void Update();
    void Delete();
private:
    StateMachine MobSM;
};

#endif
