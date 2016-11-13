#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include <string>
#include <vector>

class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    virtual void Init() = 0;
    virtual void Update() = 0;
    virtual void AddState(std::string state_name) = 0;
    virtual void DestroyState(std::string state) = 0;
    virtual std::string GetState();
    virtual void SetState(std::string state_name) = 0;
protected:
    std::vector<std::string> StateList;
};

#endif
