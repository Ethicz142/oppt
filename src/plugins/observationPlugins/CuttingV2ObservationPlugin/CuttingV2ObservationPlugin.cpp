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

    int getStochasticLowOptimalHighValue(const float& number, const float lowerBound, const float upperBound, const float& prob, const float& error) const {
        //low -> 1
        //optimal -> 2
        //high -> 3
        float observationLowerBound;
        float observationUpperBound;

        int trueObservation;
        int otherObservation1;
        int otherObservation2;
        if (number < lowerBound){
            trueObservation = 1;
            otherObservation1 = 2;
            otherObservation2 = 3;
            observationLowerBound = 0.0f;
            observationUpperBound = lowerBound;
        } 
        else if (number >= lowerBound && number <= upperBound){
            trueObservation = 2;
            otherObservation1 = 1;
            otherObservation2 = 3;
            observationLowerBound = lowerBound;
            observationUpperBound = upperBound;
        }
        else {
            trueObservation = 3;
            otherObservation1 = 1;
            otherObservation2 = 2;
            observationLowerBound = upperBound;
            observationUpperBound = 1.0f;
        }

        float cutoff = 1.0f - error * (1.0f - (observationUpperBound - observationLowerBound));

        if (prob <= cutoff) {
            return trueObservation;
        } else{
            unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
            std::default_random_engine generator(seed1);
            std::uniform_real_distribution<double> randomObservation(0.0f, 1.0f);
            if ((FloatType) randomObservation(generator) <= 0.5){
                return otherObservation1;
            } else {
                return otherObservation2;
            }
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

    FloatType getSensorCorrectnessProbability(const int& observation, const FloatType& trueCutterPropertyValue, const FloatType& lowerBound, const FloatType& upperBound, const FloatType& error) const {
        int trueCutterObservation = getLowOptimalHighValue(trueCutterPropertyValue, lowerBound, upperBound);

        float observationLowerBound;
        float observationUpperBound;

        if (observation == 1) {
            observationLowerBound = 0.0f;
            observationUpperBound = lowerBound;
        } else if (observation == 2) {
            observationLowerBound = lowerBound;
            observationUpperBound = upperBound;
        } else if (observation == 3){
            observationLowerBound = upperBound;
            observationUpperBound = 1.0f;
        } else if (observation == 0) {
            return 1.0f;
        } else {
            return 0.0f;
        }

        if (trueCutterObservation == observation) {
            return 1.0f - error * (1.0f - (observationUpperBound - observationLowerBound));
        } else {
            return error * (1.0f - (observationUpperBound - observationLowerBound)) / 2.0f;
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
        std::uniform_real_distribution<double> uniformDistribution(0.0f, 1.0f);

        float objHardnessLowerBound = cuttingV2Options_->trueObjectHardnessRange[0];
        float objHardnessUpperBound = cuttingV2Options_->trueObjectHardnessRange[1];
        float objSharpnessLowerBound = cuttingV2Options_->trueObjectSharpnessRange[0];
        float objSharpnessUpperBound = cuttingV2Options_->trueObjectSharpnessRange[1];
        if (actionVec[0] < -0.5){
            // The action is scan for hardness
            //the first observation is for D, skip it
            for (int i = 1; i < observationVec.size(); i += 2){
                // for each cutter (hardness, sharpness)
                float hardnessObservationValue = stateVec[i];

                observationVec[i] = getStochasticLowOptimalHighValue(restrictWithinRange(hardnessObservationValue, 0.0, 1.0), objHardnessLowerBound, objHardnessUpperBound, (FloatType) uniformDistribution(generator), cuttingV2Options_->hardnessErrorBound);
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
                float sharpnessObservationValue = stateVec[i + 1];
                // float sharpnessObservationValue = stateVec[i + 1];
                // debug::show_message(debug::to_string(hardnessObservationValue));
                // debug::show_message(debug::to_string(sharpnessObservationValue));
                observationVec[i+1] = getStochasticLowOptimalHighValue(restrictWithinRange(sharpnessObservationValue, 0.0, 1.0), objSharpnessLowerBound, objSharpnessUpperBound, (FloatType) uniformDistribution(generator), cuttingV2Options_->sharpnessErrorBound);
            }
        } else{
            //cutter used
            int cutterUsed = (int) (actionVec[0] + 0.25);
            int cutterIndex = 2 * cutterUsed - 1;
            float trueCutterHardness = stateVec[cutterIndex];
            float trueCutterSharpness = stateVec[cutterIndex + 1];

            //apply uncertainty first then get mapping
            float hardnessObservationValue = trueCutterHardness;
            float sharpnessObservationValue = trueCutterSharpness;

            // float hardnessObservationValue = trueCutterHardness;
            // float sharpnessObservationValue = trueCutterSharpness;
            observationVec[cutterIndex] = getStochasticLowOptimalHighValue(restrictWithinRange(hardnessObservationValue, 0.0, 1.0), objHardnessLowerBound, objHardnessUpperBound, (FloatType) uniformDistribution(generator), cuttingV2Options_->hardnessErrorBound);
            observationVec[cutterIndex+1] = getStochasticLowOptimalHighValue(restrictWithinRange(sharpnessObservationValue, 0.0, 1.0), objSharpnessLowerBound, objSharpnessUpperBound, (FloatType) uniformDistribution(generator), cuttingV2Options_->hardnessErrorBound);

        }
        ObservationSharedPtr observation = std::make_shared<DiscreteVectorObservation>(observationVec);
        observation->as<DiscreteVectorObservation>()->setBinNumber(binNumber);
        observationResult->observation = observation;
        observationResult->errorVector = observationRequest->errorVector;
        return observationResult;
    }

    virtual FloatType calcLikelihood(const RobotStateSharedPtr& state,
                                     const Action *action,
                                     const Observation *observation) const override {
        VectorFloat stateVec = state->as<VectorState>()->asVector();
        VectorFloat actionVec = action->as<VectorAction>()->asVector();
        VectorFloat observationVec = observation->as<DiscreteVectorObservation>()->asVector();

        float objHardnessLowerBound = cuttingV2Options_->trueObjectHardnessRange[0];
        float objHardnessUpperBound = cuttingV2Options_->trueObjectHardnessRange[1];
        float objSharpnessLowerBound = cuttingV2Options_->trueObjectSharpnessRange[0];
        float objSharpnessUpperBound = cuttingV2Options_->trueObjectSharpnessRange[1];


        if (actionVec[0] < -0.5) {
            float likelihood = 1.0f;
            // debug::show_message("---------- CALC -------------");
            // for (int i = 0; i < observationVec.size(); i ++){
            //     debug::show_message(debug::to_string(observationVec[i]) + " -  " + debug::to_string(stateVec[i]));
            // }

            //example obvs: 0 3 0 3 0
            //if i starts at 0, then when i is odd we get hardness
            //first hardness observation is at 1
            for (int i = 1; i < observationVec.size(); i += 2){       
                // debug::show_message(debug::to_string(observationVec[i]) + " -  " + 
                //         debug::to_string(stateVec[i]) + " = " + debug::to_string(getSensorCorrectnessProbability((unsigned int)(observationVec[i] + 0.25), stateVec[i], objHardnessLowerBound, objHardnessUpperBound, cuttingV2Options_->hardnessErrorBound)));
                likelihood = likelihood * getSensorCorrectnessProbability((unsigned int)(observationVec[i] + 0.25), stateVec[i], objHardnessLowerBound, objHardnessUpperBound, cuttingV2Options_->hardnessErrorBound);
            }
            // debug::show_message("---------- CALC ENDED -------------");
            // debug::show_message(debug::to_string(likelihood));
            return likelihood;
        }
        else if (actionVec[0] < 0.5) {
            float likelihood = 1.0f;
            // i = 2 is the start of the first sharpness observation, then it is every second one
            for (int i = 2; i < observationVec.size(); i += 2){
                //sharpness
                likelihood = likelihood * getSensorCorrectnessProbability((unsigned int)(observationVec[i] + 0.25), stateVec[i], objSharpnessLowerBound, objSharpnessUpperBound, cuttingV2Options_->sharpnessErrorBound);
            }
            return likelihood;
        } else {
            int cutterUsed = (int) (actionVec[0] + 0.25);
            int cutterIndex = 2 * cutterUsed - 1;
            float trueCutterHardness = stateVec[cutterIndex];
            float trueCutterSharpness = stateVec[cutterIndex + 1];

            float likelihood = getSensorCorrectnessProbability((unsigned int)(observationVec[cutterIndex] + 0.25), trueCutterHardness, objHardnessLowerBound, objHardnessUpperBound, cuttingV2Options_->hardnessErrorBound)
                                * getSensorCorrectnessProbability((unsigned int)(observationVec[cutterIndex + 1] + 0.25), trueCutterSharpness, objSharpnessLowerBound, objSharpnessUpperBound, cuttingV2Options_->sharpnessErrorBound);
            return likelihood;
        }

    }

private:
    CuttingV2GeneralOptions* cuttingV2Options_;
};
OPPT_REGISTER_OBSERVATION_PLUGIN(CuttingV2ObservationPlugin)
}
