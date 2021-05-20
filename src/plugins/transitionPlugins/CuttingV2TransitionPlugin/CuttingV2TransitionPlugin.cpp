#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"
#include "CuttingV2TransitionOptions.hpp"


namespace oppt
{
class CuttingV2TransitionPlugin: public TransitionPlugin
{
public:
    CuttingV2TransitionPlugin():
        TransitionPlugin() {
    }

    virtual ~CuttingV2TransitionPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        parseOptions_<CuttingV2TransitionOptions>(optionsFile);
        cuttingV2TransitionOptions_ = static_cast<CuttingV2TransitionOptions*>(options_.get());
        return true;
    }

    virtual PropagationResultSharedPtr propagateState(const PropagationRequest* propagationRequest) const override {
        // debug::show_message("Transition propagateState");
        // for (int i = 0; i < cuttingV2TransitionOptions_->trueCutterProperties.size(); i++){
        //     debug::show_message(debug::to_string(cuttingV2TransitionOptions_->trueCutterProperties[i]));
        // }
        // debug::show_message(debug::to_string(cuttingV2TransitionOptions_->trueObjectHardness));

        // Copy information from propagationRequest to propagationResult
        PropagationResultSharedPtr propagationResult(new PropagationResult());
        propagationResult->previousState = propagationRequest->currentState.get();
        propagationResult->action = propagationRequest->action;
       
        // Extract information from propagationRequest as vectors
        VectorFloat actionApplied = propagationRequest->action->as<VectorAction>()->asVector();
        VectorFloat stateVector = propagationRequest->currentState->as<VectorState>()->asVector();

        // Initialize the next state to be the same as the previous state by default
        VectorFloat resultingState(stateVector);

        // Create a robotState object from resulting state
        RobotStateSharedPtr nextRobotState = std::make_shared<oppt::VectorState>(resultingState);
        propagationResult->nextState = nextRobotState;

        return propagationResult;
    }
private:
    CuttingV2TransitionOptions* cuttingV2TransitionOptions_;
};
OPPT_REGISTER_TRANSITION_PLUGIN(CuttingV2TransitionPlugin)
}