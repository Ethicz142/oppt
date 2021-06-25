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
#include "../../problemUtils/CuttingV2GeneralOptions.hpp"

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
        parseOptions_<CuttingV2GeneralOptions>(optionsFile);
        cuttingV2Options_ = static_cast<CuttingV2GeneralOptions*>(options_.get());
        return true;
    }

    virtual FloatType getReward(const PropagationResultSharedPtr& propagationResult) const override {  
        // debug::show_message("propagationResult");
        // debug::show_message(debug::to_string(propagationResult));     

        // Retrieve next state (s') from the propagation result
        VectorFloat stateVec = propagationResult->nextState->as<VectorState>()->asVector();
        VectorFloat actionVec = propagationResult->action->as<VectorAction>()->asVector(); 

        // Check if scan
        if(actionVec[0] == 0){
            return cuttingV2Options_->scanPenalty;
        }
        // cut action + object is uncut
        else if (actionVec[0] > 0 && stateVec[0] == 0){
            return cuttingV2Options_->uncutPenalty + cuttingV2Options_->cutPenalty;
        }
        // cut action + object is cut
        else if (actionVec[0] > 0 && stateVec[0] == 1){
            return cuttingV2Options_->objectCutReward + cuttingV2Options_->cutPenalty;
        }
        // cut action + object is damaged
        else if (actionVec[0] > 0 && stateVec[0] == 2){
            return cuttingV2Options_->damagedPenalty + cuttingV2Options_->cutPenalty;
        }
        //cut action
        else{
            return cuttingV2Options_->cutPenalty;
        }

    }

    //you need this function
    virtual std::pair<FloatType, FloatType> getMinMaxReward() const override {
        return std::make_pair(cuttingV2Options_->damagedPenalty,
                              cuttingV2Options_->objectCutReward);
    }

private:
    CuttingV2GeneralOptions* cuttingV2Options_;

};
OPPT_REGISTER_REWARD_PLUGIN(CuttingV2RewardPlugin)
}
