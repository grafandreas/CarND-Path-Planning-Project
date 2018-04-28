#include "statemachine.h"



class StateMachine::impl {
public:

    class State {

    public:



         virtual void on_enter() {};
         virtual void on_exit()  {};
        virtual State * process(StateMachine::impl &impl) = 0 ;
    };

    class InitState : public State {
        virtual State * process(StateMachine::impl & impl ) {
            return & impl.forward;
        }
    };

    class ForwardState : public State {
        virtual State * process(StateMachine::impl & impl ) {
            return & impl.forward;
        }
    };

    InitState init;
    ForwardState forward;


    State * current_state;


    impl() {
        current_state = & init;
        current_state->on_enter();
    }


};



StateMachine::StateMachine() : pImpl(new StateMachine::impl)
{

}

void StateMachine::process() {
    StateMachine::impl::State * next_state = pImpl->current_state->process(*pImpl.get());
    if(next_state != pImpl->current_state) {
        pImpl->current_state->on_exit();
        pImpl->current_state = next_state;
        pImpl->current_state->on_enter();
    }
}
