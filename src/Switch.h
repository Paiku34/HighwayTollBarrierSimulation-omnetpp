//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __HIGHWAYBARRIER_SWITCH_H_
#define __HIGHWAYBARRIER_SWITCH_H_

#include <omnetpp.h>
#include <vector>
#include "ETCLane.h"
#include "StandardLane.h"
#include "Vehicle_m.h"

using namespace omnetpp;

class Switch : public cSimpleModule
{
    // Total number of lanes
    int N;

    // Number of ETC lanes
    int K;

    // Array of references to etc lanes
    std::vector<ETCLane*> EtcLanes;

    // Array of references to std lanes
    std::vector<StandardLane*> StdLanes;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;

    int chooseEtcLane();
    int chooseStdLane();
};

#endif
