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
        debug::show_message("transition");
        debug::show_message(optionsFile);
        return true;
    }

    virtual PropagationResultSharedPtr propagateState(const PropagationRequest* propagationRequest) const override {
        PropagationResultSharedPtr propagationResult(new PropagationResult());
        debug::show_message("propagationRequest");
        debug::show_message(debug::to_string(propagationRequest));
        return propagationResult;
    }

};
OPPT_REGISTER_TRANSITION_PLUGIN(TigerTransitionPlugin)
}