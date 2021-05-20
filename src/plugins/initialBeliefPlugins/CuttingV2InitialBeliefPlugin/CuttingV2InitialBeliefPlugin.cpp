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
#ifndef _CUTTINGV2_INITIAL_STATE_SAMPLER_PLUGIN_HPP_
#define _CUTTINGV2_INITIAL_STATE_SAMPLER_PLUGIN_HPP_
#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"
#include "../../problemUtils/CuttingV2GeneralOptions.hpp"

namespace oppt
{
class CuttingV2InitialBeliefPlugin: public InitialBeliefPlugin
{
public:
    CuttingV2InitialBeliefPlugin():
        InitialBeliefPlugin() {
    }

    virtual ~CuttingV2InitialBeliefPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        parseOptions_<CuttingV2GeneralOptions>(optionsFile);
        cuttingV2Options_ = static_cast<CuttingV2GeneralOptions*>(options_.get());
        return true;
    }

    virtual RobotStateSharedPtr sampleAnInitState() override {
        // debug::show_message(debug::to_string(cuttingV2Options_->numberOfCutters));

        // Update components of the resulting vector
        // Sample from uniform distribution to make the transition on the intention value
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_real_distribution<double> distribution(0, 1);

        int numberOfCutters = cuttingV2Options_->numberOfCutters;
        //each cutter has a sharpness and hardness state, the object also has a state
        int numberOfStates = 2 * numberOfCutters + 1;
        
        // Allocate a random state based on sample
        VectorFloat initialStateVec(numberOfStates, 0);

        //the object is uncut (0), hence why we start at i = 1
        //random sharpness & hardness for the cutters, we don't know what they are until we scan
        for (int i = 1; i < numberOfStates; i++){
            initialStateVec[i] = (FloatType) distribution(generator);
        }

        // Wrap initial state vector into oppt RobotState structure
        RobotStateSharedPtr initialState = std::make_shared<VectorState>(initialStateVec);

        return initialState;
    }
private:
    CuttingV2GeneralOptions* cuttingV2Options_;
};

OPPT_REGISTER_INITIAL_BELIEF_PLUGIN(CuttingV2InitialBeliefPlugin)

}

#endif