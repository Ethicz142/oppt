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
#ifndef _TIGER_INITIAL_STATE_SAMPLER_PLUGIN_HPP_
#define _TIGER_INITIAL_STATE_SAMPLER_PLUGIN_HPP_
#include "oppt/plugin/Plugin.hpp"
#include "oppt/opptCore/Distribution.hpp"

namespace oppt
{
class TigerInitialBeliefPlugin: public InitialBeliefPlugin
{
public:
    TigerInitialBeliefPlugin():
        InitialBeliefPlugin() {
    }

    virtual ~TigerInitialBeliefPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        return true;
    }

    virtual RobotStateSharedPtr sampleAnInitState() override {
        // debug::show_message("sample state called from belief plugin");

        // Update components of the resulting vector
        // Sample from uniform distribution to make the transition on the intention value
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_real_distribution<double> distribution(0, 1);

        
        // Allocate a random state based on sample
        VectorFloat initialStateVec(1, 1);

        // Pack it into the oppt structures
        FloatType tigerRightSample = (FloatType) distribution(generator);
        // Change according to sample
        if(tigerRightSample >= 0.5){
            // Init the state with tiger right
            initialStateVec[0] = 2;
        }

        // Wrap initial state vector into oppt RobotState structure
        RobotStateSharedPtr initialState = std::make_shared<VectorState>(initialStateVec);

        return initialState;
    }
};

OPPT_REGISTER_INITIAL_BELIEF_PLUGIN(TigerInitialBeliefPlugin)

}

#endif