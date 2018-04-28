#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include <memory>

class StateMachine
{
public:
    StateMachine();
    void process();

private:

    class impl;
    std::unique_ptr<impl> pImpl;

};

#endif // STATEMACHINE_H
