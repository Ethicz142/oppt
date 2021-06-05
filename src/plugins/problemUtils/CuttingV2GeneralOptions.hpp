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
#ifndef _CUTTINGV2_UTILS_HPP_
#define _CUTTINGV2_UTILS_HPP_
#include "oppt/plugin/PluginOptions.hpp"

namespace oppt
{

// Options object to parse information from configuration file
class CuttingV2GeneralOptions: public PluginOptions
{
public:
    CuttingV2GeneralOptions() = default;

    virtual ~CuttingV2GeneralOptions() = default;

    VectorFloat trueObjectHardnessRange;
    VectorFloat trueObjectSharpnessRange;
    int numberOfCutters;

    // observation variables
    FloatType damageErrorBound = 0.0;
    FloatType sharpnessErrorBound = 0.0;
    FloatType hardnessErrorBound = 0.0;

    //reward variables
    FloatType objectCutReward = 0.0;
    FloatType damagedPenalty = 0.0;
    FloatType scanPenalty = 0.0;
    FloatType cutPenalty = 0.0;

    //initial belief variables
    int numberOfSuitableCutters = 0;

    static std::unique_ptr<options::OptionParser> makeParser() {
        std::unique_ptr<options::OptionParser> parser =
            PluginOptions::makeParser();
        addGeneralPluginOptions(parser.get());
        return std::move(parser);
    }

    // Add the transition plugin options
    static void addGeneralPluginOptions(options::OptionParser* parser) {

        //general options
        parser->addOption<VectorFloat>("generalOptions",
                                        "trueObjectHardnessRange",
                                        &CuttingV2GeneralOptions::trueObjectHardnessRange); 
        parser->addOption<VectorFloat>("generalOptions",
                                        "trueObjectSharpnessRange",
                                        &CuttingV2GeneralOptions::trueObjectSharpnessRange);    
        parser->addOption<int>("generalOptions",
                                    "numberOfCutters",
                                    &CuttingV2GeneralOptions::numberOfCutters);

        //observation
        parser->addOption<FloatType>("observationPluginOptions",
                                        "damageErrorBound",
                                        &CuttingV2GeneralOptions::damageErrorBound);
        parser->addOption<FloatType>("observationPluginOptions",
                                        "sharpnessErrorBound",
                                        &CuttingV2GeneralOptions::sharpnessErrorBound);
        parser->addOption<FloatType>("observationPluginOptions",
                                        "hardnessErrorBound",
                                        &CuttingV2GeneralOptions::hardnessErrorBound);

        //reward

        parser->addOption<FloatType>("rewardPluginOptions",
                                        "objectCutReward",
                                        &CuttingV2GeneralOptions::objectCutReward);
        parser->addOption<FloatType>("rewardPluginOptions",
                                        "damagedPenalty",
                                        &CuttingV2GeneralOptions::damagedPenalty);
        parser->addOption<FloatType>("rewardPluginOptions",
                                        "scanPenalty",
                                        &CuttingV2GeneralOptions::scanPenalty);
        parser->addOption<FloatType>("rewardPluginOptions",
                                        "cutPenalty",
                                        &CuttingV2GeneralOptions::cutPenalty);

        //intial belief

        parser->addOption<int>("initialBeliefPluginOptions",
                                        "numberOfSuitableCutters",
                                        &CuttingV2GeneralOptions::numberOfSuitableCutters);
    }
};
}

#endif