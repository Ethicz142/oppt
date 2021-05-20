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
#include "oppt/opptCore/Distribution.hpp"
#include "../../problemUtils/CuttingV2GeneralOptions.hpp"

namespace oppt
{
class CuttingV2ObservationPlugin: public ObservationPlugin
{
public :
    CuttingV2ObservationPlugin():
        ObservationPlugin() {
    }

    virtual bool load(const std::string& optionsFile) override {
        parseOptions_<CuttingV2GeneralOptions>(optionsFile);
        cuttingV2Options_ = static_cast<CuttingV2GeneralOptions*>(options_.get());
        return true;
    }

    virtual ObservationResultSharedPtr getObservation(const ObservationRequest* observationRequest) const override {
        // debug::show_message("observationRequest");
        // debug::show_message(debug::to_string(cuttingV2Options_->numberOfCutters));
        ObservationResultSharedPtr observationResult = std::make_shared<ObservationResult>();
        VectorFloat stateVec = observationRequest->currentState->as<VectorState>()->asVector();
        VectorFloat actionVec = observationRequest->action->as<VectorAction>()->asVector();
        VectorFloat observationVec(robotEnvironment_->getRobot()->getObservationSpace()->getNumDimensions(), 0.0);
        long binNumber = 0;

        // Populate observation value according to action taken
        // Sample from uniform distribution to get noisy observation from state and action
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed1);
        std::uniform_real_distribution<double> hardnessDistribution(-cuttingV2Options_->hardnessBound, cuttingV2Options_->hardnessBound);
        std::uniform_real_distribution<double> sharpnessDistribution(-cuttingV2Options_->sharpnessBound, cuttingV2Options_->sharpnessBound);
        if (actionVec[0] < 0.5){
            // The action is scan
            //the first observation is for D, skip it
            for (int i = 1; i < observationVec.size(); i += 2){
                // for each cutter (hardness, sharpness)
                observationVec[i] = cuttingV2Options_->trueCutterProperties[i - 1] + (FloatType) hardnessDistribution(generator);
                observationVec[i+1] = cuttingV2Options_->trueCutterProperties[i] + (FloatType) sharpnessDistribution(generator);
            }
        } else{
            FloatType probability = 1 - cuttingV2Options_->observationError;
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

private:
    CuttingV2GeneralOptions* cuttingV2Options_;
};
OPPT_REGISTER_OBSERVATION_PLUGIN(CuttingV2ObservationPlugin)
}
