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
    void Delete();
private:
    bool DeadAlly;

};

#endif
