#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"
#include "../../problemUtils/CuttingV2GeneralOptions.hpp"


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
        parseOptions_<CuttingV2GeneralOptions>(optionsFile);
        cuttingV2Options_ = static_cast<CuttingV2GeneralOptions*>(options_.get());
        return true;
    }

    virtual PropagationResultSharedPtr propagateState(const PropagationRequest* propagationRequest) const override {
        // debug::show_message("Transition propagateState");
        // for (int i = 0; i < cuttingV2Options_->trueCutterProperties.size(); i++){
        //     debug::show_message(debug::to_string(cuttingV2Options_->trueCutterProperties[i]));
        // }
        // debug::show_message(debug::to_string(cuttingV2Options_->trueObjectHardness));

        // Copy information from propagationRequest to propagationResult
        PropagationResultSharedPtr propagationResult(new PropagationResult());
        propagationResult->previousState = propagationRequest->currentState.get();
        propagationResult->action = propagationRequest->action;
       
        // Extract information from propagationRequest as vectors
        VectorFloat actionApplied = propagationRequest->action->as<VectorAction>()->asVector();
        VectorFloat stateVector = propagationRequest->currentState->as<VectorState>()->asVector();

        // Initialize the next state to be the same as the previous state by default
        VectorFloat resultingState(stateVector);
        
        //not scan, scan doesn't change the state
        if (actionApplied[0] > 0){
            int cutterUsed = (int) (actionApplied[0] + 0.25);
            int cutterIndex = 2 * cutterUsed - 1;
            float trueCutterHardness = stateVector[cutterIndex];
            float trueCutterSharpness = stateVector[cutterIndex + 1];

            float objHardnessLowerBound = cuttingV2Options_->trueObjectHardnessRange[0];
            float objHardnessUpperBound = cuttingV2Options_->trueObjectHardnessRange[1];
            float objSharpnessLowerBound = cuttingV2Options_->trueObjectSharpnessRange[0];
            float objSharpnessUpperBound = cuttingV2Options_->trueObjectSharpnessRange[1];

            unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed1);
            std::uniform_real_distribution<double> distribution(0, 1);

            FloatType sample = (FloatType) distribution(generator);
            // if (actionApplied[0] > 1.5){
            //     debug::show_message("------------");
            //     debug::show_message(debug::to_string(actionApplied[0]));
            //     debug::show_message(debug::to_string(trueCutterHardness));
            //     debug::show_message(debug::to_string(trueCutterSharpness));
            //     debug::show_message(debug::to_string(objHardnessLowerBound));
            //     debug::show_message(debug::to_string(objHardnessUpperBound));
            //     debug::show_message(debug::to_string(objSharpnessLowerBound));
            //     debug::show_message(debug::to_string(objSharpnessUpperBound));
            // }
            
            
            if (trueCutterHardness >= objHardnessLowerBound && trueCutterSharpness >= objSharpnessLowerBound){
                // hardness & sharpness in optimal range
                if (trueCutterHardness <= objHardnessUpperBound && trueCutterSharpness <= objSharpnessUpperBound){
                    // debug::show_message("optimal");
                    if (sample <= 0.90){
                        resultingState[0] = 1;
                    }                    
                }
                //high sharpness, optimal hardness
                else if (trueCutterHardness <= objHardnessUpperBound && trueCutterSharpness > objSharpnessUpperBound){
                    if (sample <= 0.7){
                        resultingState[0] = 1;
                    } else{
                        resultingState[0] = 2;
                    }                   
                }
                //high hardness, optimal sharpness
                else if (trueCutterHardness > objHardnessUpperBound && trueCutterSharpness <= objSharpnessUpperBound){
                    if (sample <= 0.7){
                        resultingState[0] = 1;
                    } else{
                        resultingState[0] = 2;
                    }                   
                }
                //high hardness & sharpness
                else if (trueCutterHardness > objHardnessUpperBound && trueCutterSharpness > objSharpnessUpperBound){
                    if (sample <= 0.9){
                        resultingState[0] = 1;
                    } else{
                        resultingState[0] = 2;
                    }                   
                }
            }
            else if (trueCutterHardness >= objHardnessLowerBound && trueCutterSharpness < objSharpnessLowerBound){
                //low sharpness, optimal hardness
                if (trueCutterHardness <= objHardnessUpperBound){
                    if (sample <= 0.7){
                        resultingState[0] = 2;
                    }  
                }
                //low sharpness, high hardness
                if (trueCutterHardness > objHardnessUpperBound){
                    if (sample <= 0.9){
                        resultingState[0] = 2;
                    }  
                }
            }
            else if (trueCutterHardness < objHardnessLowerBound && trueCutterSharpness >= objSharpnessLowerBound){
                //low hardness, optimal sharpness
                if (trueCutterSharpness <= objSharpnessUpperBound){
                    if (sample <= 0.3){
                        resultingState[0] = 2;
                    }  
                }
                //low hardness, high sharpness
                if (trueCutterSharpness > objSharpnessUpperBound){
                    if (sample <= 0.3){
                        resultingState[0] = 1;
                    }  
                }
            }
            else {
                //low hardness and low sharpness
                if (sample <= 0.1){
                    resultingState[0] = 2;
                }  
            }

        }

        // Create a robotState object from resulting state
        RobotStateSharedPtr nextRobotState = std::make_shared<oppt::VectorState>(resultingState);
        propagationResult->nextState = nextRobotState;

        return propagationResult;
    }
private:
    CuttingV2GeneralOptions* cuttingV2Options_;
};
OPPT_REGISTER_TRANSITION_PLUGIN(CuttingV2TransitionPlugin)
}