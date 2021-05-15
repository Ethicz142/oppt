#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"

namespace oppt
{
class TigerTransitionPlugin: public TransitionPlugin
{
public:
    TigerTransitionPlugin():
        TransitionPlugin() {
    }

    virtual ~TigerTransitionPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        // debug::show_message("transition");
        // debug::show_message(optionsFile);
        return true;
    }

    virtual PropagationResultSharedPtr propagateState(const PropagationRequest* propagationRequest) const override {
        // debug::show_message("Transition propagateState");
        // debug::show_message(debug::to_string(propagationRequest));

        // Copy information from propagationRequest to propagationResult
        PropagationResultSharedPtr propagationResult(new PropagationResult());
        propagationResult->previousState = propagationRequest->currentState.get();
        propagationResult->action = propagationRequest->action;
       
        // Extract information from propagationRequest as vectors
        VectorFloat actionApplied = propagationRequest->action->as<VectorAction>()->asVector();
        VectorFloat stateVector = propagationRequest->currentState->as<VectorState>()->asVector();

        // Initialize the next state to be the same as the previous state by default
        VectorFloat resultingState(stateVector);

        // Opened a door, end the scenario
        if (actionApplied[0] < 2.25) resultingState[0] = 3.0;

        // Create a robotState object from resulting state
        RobotStateSharedPtr nextRobotState = std::make_shared<oppt::VectorState>(resultingState);
        propagationResult->nextState = nextRobotState;

        return propagationResult;
    }

};
OPPT_REGISTER_TRANSITION_PLUGIN(TigerTransitionPlugin)
}