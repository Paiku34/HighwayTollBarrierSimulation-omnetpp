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

#include "Switch.h"

Define_Module(Switch);

void Switch::initialize()
{
    // Get reference to parent module
    cModule* barrierModule = getParentModule();

    // Get total number of lanes
    N = barrierModule->par("N");

    // Get number of ETC lanes
    K = barrierModule->par("K");

    // Get and save reference to barrier etc lanes
    for(int i = 0; i < K; i++){

        // Get reference to i-th etcLan submodule of Barrier
        cModule* etcLane = barrierModule->getSubmodule("etcLane", i);

        // Add module reference to vector
        EtcLanes.push_back(check_and_cast<ETCLane*>(etcLane));
    }

    // Get and save reference to barrier std lanes
    for(int i = 0; i < (N - K); i++){

        // Get reference to i-th stdLane submodule of Barrier
        cModule* stdLane = barrierModule->getSubmodule("stdLane", i);

        // Add module reference to vector
        StdLanes.push_back(check_and_cast<StandardLane*>(stdLane));
    }
}

void Switch::handleMessage(cMessage *msg)
{
    // All incoming messages are vehicle msg from highway module
    Vehicle* vehicle = check_and_cast<Vehicle*>(msg);

    // Log arriving vehicle
    EV_INFO << "Switch::handleMessage - vehicle arrived : " << (vehicle->getHasEtc() ? "with ETC" : "without ETC") << endl;

    // If there aren't any ETC lanes OR vehicle isn't equipped with etc
    if( K == 0 || !vehicle->getHasEtc() ){

        // Choose the std lane with the least vehicles queued
        int chosenLane = chooseStdLane();

        // Send vehicle to the chosen lane
        send(msg, "out_std", chosenLane);

        EV_INFO << "Switch::handleMessage - vehicle sent to standard lane with index " << chosenLane << endl;
    }

    // If K > 0 (there are etc lanes) and vehicle has etc send it to etc lane
    else {

        // Choose the etc lane with the least vehicles queued
        int chosenLane = chooseEtcLane();

        // Send vehicle to the chosen lane
        send(msg, "out_etc", chosenLane);

        EV_INFO << "Switch::handleMessage - vehicle sent to etc lane with index " << chosenLane << endl;
    }
}

/**
 * Choose the etc lane with the least vehicles queued.
 */
int Switch::chooseEtcLane(){

    // Loops over all the etc lanes and saves the one with the least amount of vehicle queued up
    // If two or more lanes have the same amount of vehicles choose the first one

    EV_DETAIL << "Switch::chooseEtcLane - {";

    int chosenLaneIndex = 0;
    int minVehicles = EtcLanes[0]->getVehiclesQueued();

    EV_DETAIL << EtcLanes[0]->getVehiclesQueued();

    for(int i = 1; i < K; i++){

        EV_DETAIL << ", " << EtcLanes[i]->getVehiclesQueued();

        if(EtcLanes[i]->getVehiclesQueued() < minVehicles){
            chosenLaneIndex = i;
            minVehicles = EtcLanes[i]->getVehiclesQueued();
        }
    }

    EV_DETAIL << "}" << endl;
    EV_DETAIL << "Switch::chooseEtcLane - chosen lane: " << chosenLaneIndex << " (" << minVehicles << " cars)" << endl;

    return chosenLaneIndex;
}

/**
 * Choose the standard lane with the least vehicles queued
 */
int Switch::chooseStdLane(){

    // Loops over all the std lanes and saves the one with the least amount of vehicle queued up
    // If two or more lanes have the same amount of vehicles choose the first one

    EV_DETAIL << "Switch::chooseStdLane - {";

    int chosenLaneIndex = 0;
    int minVehicles = StdLanes[0]->getVehiclesQueued();

    EV_DETAIL << StdLanes[0]->getVehiclesQueued();

    for(int i = 1; i < (N - K); i++){

        EV_DETAIL << ", " << StdLanes[i]->getVehiclesQueued();

        if(StdLanes[i]->getVehiclesQueued() < minVehicles){
            chosenLaneIndex = i;
            minVehicles = StdLanes[i]->getVehiclesQueued();
        }
    }

    EV_DETAIL << "}" << endl;
    EV_DETAIL << "Switch::chooseStdLane - chosen lane: " << chosenLaneIndex << " (" << minVehicles << " cars)" << endl;

    return chosenLaneIndex;
}
