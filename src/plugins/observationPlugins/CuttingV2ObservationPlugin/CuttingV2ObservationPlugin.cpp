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

    FloatType sigmoid(const float& hardness, const float& sharpness, const float& error) const {
        //sigmoid to have the number between 0 and 1, 
        // when hardness + sharpness = 2, y ~= 2
        // when hardness + sharpness = 0, y ~= 0
        return 1.0 / (1 + std::exp(-6.0 * (hardness + sharpness + error) + 7.0));
    }

    FloatType restrictWithinRange(const float& number, const float lowerBound, const float upperBound) const {
        //have the number be between 0 and 1
        return std::max(lowerBound, std::min(upperBound, number));
    }

    int getLowOptimalHighValue(const float& number, const float lowerBound, const float upperBound) const {
        //low -> 1
        //optimal -> 2
        //high -> 3
        if (number < lowerBound){
            return 1;
        } 
        else if (number >= lowerBound && number <= upperBound){
            return 2;
        }
        else {
            return 3;
        }
    }

    int getCutterObservation(const float& sample, const float& probability, const int correctObservation, const std::vector<int>& incorrectObservations) const{
        if (sample <= probability){
            return correctObservation;
        }
        else {
            return incorrectObservations[std::rand() % incorrectObservations.size()];
        }
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
        std::uniform_real_distribution<double> hardnessDistribution(-cuttingV2Options_->hardnessErrorBound, cuttingV2Options_->hardnessErrorBound);
        std::uniform_real_distribution<double> sharpnessDistribution(-cuttingV2Options_->sharpnessErrorBound, cuttingV2Options_->sharpnessErrorBound);
        std::uniform_real_distribution<double> damageDistribution(-cuttingV2Options_->damageErrorBound, cuttingV2Options_->damageErrorBound);

        float objHardnessLowerBound = cuttingV2Options_->trueObjectHardnessRange[0];
        float objHardnessUpperBound = cuttingV2Options_->trueObjectHardnessRange[1];
        float objSharpnessLowerBound = cuttingV2Options_->trueObjectSharpnessRange[0];
        float objSharpnessUpperBound = cuttingV2Options_->trueObjectSharpnessRange[1];
        if (actionVec[0] < -0.5){
            // The action is scan for hardness
            //the first observation is for D, skip it
            for (int i = 1; i < observationVec.size(); i += 2){
                // for each cutter (hardness, sharpness)
                float hardnessObservationValue = stateVec[i] + (FloatType) hardnessDistribution(generator);
                // float hardnessObservationValue = stateVec[i];

                observationVec[i] = getLowOptimalHighValue(restrictWithinRange(hardnessObservationValue, 0.0, 1.0), objHardnessLowerBound, objHardnessUpperBound);
            }
        }
        else if (actionVec[0] < 0.5){
            // debug::show_message("== SCANNING ==");
            // for (int i = 0; i < stateVec.size(); i ++){
            //     debug::show_message(debug::to_string(stateVec[i]));
            // }

            // The action is scan for sharpness
            //the first observation is for D, skip it
            for (int i = 1; i < observationVec.size(); i += 2){
                // for each cutter (hardness, sharpness)
                float sharpnessObservationValue = stateVec[i + 1] + (FloatType) sharpnessDistribution(generator);
                // float sharpnessObservationValue = stateVec[i + 1];
                // debug::show_message(debug::to_string(hardnessObservationValue));
                // debug::show_message(debug::to_string(sharpnessObservationValue));
                observationVec[i+1] = getLowOptimalHighValue(restrictWithinRange(sharpnessObservationValue, 0.0, 1.0), objSharpnessLowerBound, objSharpnessUpperBound);
            }
        } else{
            //cutter used
            int cutterUsed = (int) actionVec[0] + 0.25;
            int cutterIndex = 2 * cutterUsed - 1;
            float trueCutterHardness = stateVec[cutterIndex];
            float trueCutterSharpness = stateVec[cutterIndex + 1];

            //apply uncertainty first then get mapping
            float hardnessObservationValue = trueCutterHardness + (FloatType) hardnessDistribution(generator);
            float sharpnessObservationValue = trueCutterSharpness + (FloatType) sharpnessDistribution(generator);

            // float hardnessObservationValue = trueCutterHardness;
            // float sharpnessObservationValue = trueCutterSharpness;
            observationVec[cutterIndex] = getLowOptimalHighValue(restrictWithinRange(hardnessObservationValue, 0.0, 1.0), objHardnessLowerBound, objHardnessUpperBound);
            observationVec[cutterIndex+1] = getLowOptimalHighValue(restrictWithinRange(sharpnessObservationValue, 0.0, 1.0), objSharpnessLowerBound, objSharpnessUpperBound);

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
