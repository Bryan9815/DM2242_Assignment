#include "StateMachine.h"
State::State() : Name(""), active(false)
{
}

State::~State()
{
}

StateMachine::StateMachine()
{
}


StateMachine::~StateMachine()
{
    while (StateList.size() > 0)
    {
        State* temp = StateList.back();
        delete temp;
        StateList.pop_back();
    }
}

void StateMachine::Init()
{

}
void StateMachine::Update()
{

}
void StateMachine::AddState(std::string state_name)
{
    State *temp = new State();
    temp->Name = state_name;
    temp->active = false;
    StateList.push_back(temp);
}
void StateMachine::DestroyState(std::string state)
{
    for (std::vector<State*>::iterator it = StateList.begin(); it != StateList.end(); ++it)
    {
        if ((*it)->Name == state)
        {
            State* temp = (*it);
            delete temp;
            StateList.erase(it);
            break;
        }
    }
}
std::string StateMachine::GetState()
{
    for (std::vector<State*>::iterator it = StateList.begin(); it != StateList.end(); ++it)
    {
        if ((*it)->active)
        {
            return (*it)->Name;
        }
    }
    return "";
}
void StateMachine::SetState(std::string state_name)
{
    if (GetState() == state_name)
        return;
    for (std::vector<State*>::iterator it = StateList.begin(); it != StateList.end(); ++it)
    {
        if ((*it)->Name == state_name)
        {
            for (std::vector<State*>::iterator it2 = StateList.begin(); it2 != StateList.end(); ++it2)
            {
                if ((*it2)->active)
                {
                    (*it)->active = true;
                    (*it2)->active = false;
                    return;
                }
            }
            (*it)->active = true;
            return;
        }
    }
}