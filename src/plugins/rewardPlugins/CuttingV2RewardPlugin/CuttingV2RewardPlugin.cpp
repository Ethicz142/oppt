/**
 * Copyright 2017
 * 
 * This file is part of On-line POMDP Planning Toolkit (OPPT).
 * OPPT is free software: you can redistribute it and/or modify it under the terms of the 
 * GNU General Public License published by the Free Software Foundation, 
 * either version 2 of the License, or (at your option) any later version.
 * 
 * OPPT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with OPPT. 
 * If not, see http://www.gnu.org/licenses/.
 */

#include "oppt/plugin/Plugin.hpp"
#include "CuttingV2RewardOptions.hpp"

namespace oppt
{
class CuttingV2RewardPlugin: public RewardPlugin
{
public:
    CuttingV2RewardPlugin():
        RewardPlugin() {

    }

    virtual ~CuttingV2RewardPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        // debug::show_message("reward");
        // debug::show_message(optionsFile);
        parseOptions_<CuttingV2RewardOptions>(optionsFile);
        cuttingV2RewardOptions_ = static_cast<CuttingV2RewardOptions*>(options_.get());
        return true;
    }

    virtual FloatType getReward(const PropagationResultSharedPtr& propagationResult) const override {  
        // debug::show_message("propagationResult");
        // debug::show_message(debug::to_string(propagationResult));     

        // Retrieve next state (s') from the propagation result
        VectorFloat stateVec = propagationResult->nextState->as<VectorState>()->asVector();
        VectorFloat actionVec = propagationResult->action->as<VectorAction>()->asVector(); 

        // Check if there was a correct guess on the left door
        if(actionVec[0] == 1){
            if(stateVec[0] == 1){
                return cuttingV2RewardOptions_->correctGuessReward;
            } else {
                return cuttingV2RewardOptions_->wrongGuessPenalty;
            }
        }

        // Check if there was a correct guess on the right door
        if(actionVec[0] == 2){
            if(stateVec[0] == 2){
                return cuttingV2RewardOptions_->correctGuessReward;
            } else {
                return cuttingV2RewardOptions_->wrongGuessPenalty;
            }
        }

        // Otherwise, apply a step penalty
        return cuttingV2RewardOptions_->stepPenalty;

    }

    //you need this function
    virtual std::pair<FloatType, FloatType> getMinMaxReward() const override {
        return std::make_pair(cuttingV2RewardOptions_->wrongGuessPenalty,
                              cuttingV2RewardOptions_->correctGuessReward);
    }

private:
    CuttingV2RewardOptions* cuttingV2RewardOptions_;

};
OPPT_REGISTER_REWARD_PLUGIN(CuttingV2RewardPlugin)
}
