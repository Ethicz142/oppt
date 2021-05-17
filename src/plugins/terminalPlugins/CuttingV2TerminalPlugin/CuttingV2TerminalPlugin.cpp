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
class CuttingV2TerminalPlugin: public TerminalPlugin
{
public:
    CuttingV2TerminalPlugin():
        TerminalPlugin() {
    }

    virtual ~CuttingV2TerminalPlugin() = default;

    virtual bool load(const std::string& optionsFile) override {
        // debug::show_message("terminal");
        // debug::show_message(optionsFile);
        return true;
    }

    virtual ValidityReportSharedPtr isValid(const PropagationResultSharedPtr& propagationResult) override {
        ValidityReportSharedPtr validityReport(new ValidityReport(propagationResult->nextState));
        validityReport->satisfiesConstraints = true;
        validityReport->isValid = true;
        return validityReport;
    }

    virtual bool isTerminal(const PropagationResultSharedPtr& propagationResult) override {
        /*** Need to guard against nullptr since Terminal Plugin is used initially.
        // Check in ProblemEnvironment.hpp
        // run(const unsigned int& run, std::ofstream & os, int argc, char const * argv[])
        ***/
    //    debug::show_message("terminal isTerminal");

        if (propagationResult->action != nullptr){
            // Retrieve the actionVec
            VectorFloat actionVec = propagationResult->action->as<VectorAction>()->asVector(); 
             // Check if an open action was taken 
            if(actionVec[0] != 3){
                // The problem is over after an open action. We consider this terminal
                return true;
            }
        }

        // No Open action was taken
        return false;
    }
    
};

OPPT_REGISTER_TERMINAL_PLUGIN(CuttingV2TerminalPlugin)
}
