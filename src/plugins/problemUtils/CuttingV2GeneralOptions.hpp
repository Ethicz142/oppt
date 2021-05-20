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

    float trueObjectHardness;
    VectorFloat trueCutterProperties;
    int numberOfCutters;

    // observation variables
    FloatType observationError = 0.0;
    FloatType sharpnessBound = 0.0;
    FloatType hardnessBound = 0.0;

    static std::unique_ptr<options::OptionParser> makeParser() {
        std::unique_ptr<options::OptionParser> parser =
            PluginOptions::makeParser();
        addGeneralPluginOptions(parser.get());
        return std::move(parser);
    }

    // Add the transition plugin options
    static void addGeneralPluginOptions(options::OptionParser* parser) {

        parser->addOption<float>("generalOptions",
                                "trueObjectHardness",
                                &CuttingV2GeneralOptions::trueObjectHardness);    
        parser->addOption<VectorFloat>("generalOptions",
                                       "trueCutterProperties",
                                       &CuttingV2GeneralOptions::trueCutterProperties);

        parser->addOption<int>("generalOptions",
                                    "numberOfCutters",
                                    &CuttingV2GeneralOptions::numberOfCutters);

        parser->addOption<FloatType>("observationPluginOptions",
                                        "observationError",
                                        &CuttingV2GeneralOptions::observationError);
        parser->addOption<FloatType>("observationPluginOptions",
                                        "sharpnessBound",
                                        &CuttingV2GeneralOptions::sharpnessBound);
        parser->addOption<FloatType>("observationPluginOptions",
                                        "hardnessBound",
                                        &CuttingV2GeneralOptions::hardnessBound);
    }
};
}

#endif