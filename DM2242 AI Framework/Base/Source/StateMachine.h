#ifndef BASE_ENTITY_H
#define BASE_ENTITY_H

#include <string>
#include <vector>

class State
{
public:
    State();
    ~State();

    std::string Name;
    bool active;
};


class StateMachine
{
public:
    StateMachine();
    ~StateMachine();

    void Init();
    void Update();
    void AddState(std::string state_name);
    void DestroyState(std::string state);
    std::string GetState();
    void SetState(std::string state_name);
private:
    std::vector<State*> StateList;
};




#endif
