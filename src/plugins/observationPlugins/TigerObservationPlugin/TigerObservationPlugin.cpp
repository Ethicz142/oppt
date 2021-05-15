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

namespace oppt
{
class TigerObservationPlugin: public ObservationPlugin
{
public :
    TigerObservationPlugin():
        ObservationPlugin() {
    }

    virtual bool load(const std::string& optionsFile) override {
        // debug::show_message("observation");
        // debug::show_message(optionsFile);
        return true;
    }

    virtual ObservationResultSharedPtr getObservation(const ObservationRequest* observationRequest) const override {
        // debug::show_message("observationRequest");
        // debug::show_message(debug::to_string(observationRequest));
        ObservationResultSharedPtr observationResult = std::make_shared<ObservationResult>();
        VectorFloat stateVec = observationRequest->currentState->as<VectorState>()->asVector();
        VectorFloat actionVec = observationRequest->action->as<VectorAction>()->asVector();
        VectorFloat observationVec(robotEnvironment_->getRobot()->getObservationSpace()->getNumDimensions(), 0.0);
        long binNumber = 0;

        if (actionVec[0] < 2.5){
            // The action is a movement action, so no sampling here. An observation of 0 indicates a null observation
            observationVec[0] = 0.0;
        } else{
            FloatType probability = 0.85;
            bool obsMatches =
                std::bernoulli_distribution(
                    probability)(*(robotEnvironment_->getRobot()->getRandomEngine().get()));
            int stateInt = (int) stateVec[0] + 0.25;
            if (obsMatches) {
                observationVec[0] = stateInt;
            } else {
                observationVec[0] = stateInt^3;
            }        
            binNumber = (int) observationVec[0] + 0.25;
        }
        ObservationSharedPtr observation = std::make_shared<DiscreteVectorObservation>(observationVec);
        observation->as<DiscreteVectorObservation>()->setBinNumber(binNumber);
        observationResult->observation = observation;
        observationResult->errorVector = observationRequest->errorVector;
        return observationResult;
    }

};
OPPT_REGISTER_OBSERVATION_PLUGIN(TigerObservationPlugin)
}
