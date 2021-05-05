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
        debug::show_message("observation");
        debug::show_message(optionsFile);
        return true;
    }

    virtual ObservationResultSharedPtr getObservation(const ObservationRequest* observationRequest) const override {
        ObservationResultSharedPtr observationResult = std::make_shared<ObservationResult>();
        debug::show_message("observationRequest");
        debug::show_message(debug::to_string(observationRequest));
        return observationResult;
    }

};
OPPT_REGISTER_OBSERVATION_PLUGIN(TigerObservationPlugin)
}
