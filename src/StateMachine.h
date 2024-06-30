#pragma once
#include <map>
#include <functional>
#include "Stack.h"

template <typename TState, typename TInputType>
class StateMachine
{
    Stack<TState> stateStack;
public:
    std::string input;
    std::map<TState, std::function<TState(StateMachine<TState, TInputType>&, void*, TInputType)>> transitions;
    std::map<TState, std::function<TState(StateMachine<TState, TInputType>&)>> autoTransitions;

    StateMachine(TState startState) : stateStack() { stateStack.Push(startState); }
    void SetInput(void* data, TInputType type)
    {
        SetState(transitions.at(stateStack.Top())(*this, data, type));

        if (autoTransitions.contains(stateStack.Top()))
        {
            SetState(autoTransitions.at(stateStack.Top())(*this));
        }
    }
    void PushState(TState state)
    {
        if (state == stateStack.Top()) return;
        stateStack.Push(state);
    }
    void PopState()
    {
        TState oldState = stateStack.Pop();
    }
    void SetState(TState newState)
    {
        if (newState != stateStack.Top() && (int) newState != 0)
        {
            stateStack.Pop();
            stateStack.Push(newState);
        }
    }

};