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

#include "Highway.h"

Define_Module(Highway);

void Highway::initialize()
{
    // Save parameters
    p = par("p");
    Tc = par("Tc");
    Tt = par("Tt");

    // Initialize timers
    timerCar_ = new cMessage("timerCar");
    timerTruck_ = new cMessage("timerTruck");

    // Schedule timer for cars
    scheduleTimer(true);

    // Schedule timer for trucks
    scheduleTimer(false);
}
void Highway::handleMessage(cMessage *msg)
{
    // Check timer type (car or truck)
    if(msg->isName("timerCar")){

        EV_INFO << "Highway::handleMessage - timer Car timeout" << endl;

        // Generate and send car
        generateVehicle(true);

        // Reschedule timer for cars
        scheduleTimer(true);
    }

    else{

        EV_INFO << "Highway::handleMessage - timer Truck timeout" << endl;

        // Generate and send truck
        generateVehicle(false);

        // Reschedule timer for trucks
        scheduleTimer(false);
    }
}

void Highway::scheduleTimer(bool isCar){

    if(isCar){

        // Get new waiting time for car
        simtime_t waitingTime = exponential(Tc, CAR_RNG);

        // reschedule car timer
        scheduleAt(simTime() + waitingTime, timerCar_);

        EV_INFO << "Highway::scheduleTimer - car timer scheduled at +" << waitingTime << endl;
    }
    else{

        // Get new waiting time for truck
        simtime_t waitingTime = exponential(Tt, TRUCK_RNG);

        // reschedule truck timer
        scheduleAt(simTime() + waitingTime, timerTruck_);

        EV_INFO << "Highway::scheduleTimer - truck timer scheduled at +" << waitingTime << endl;
    }

}

void Highway::generateVehicle(bool isCar){

    // Generate new vehicle message
    Vehicle* vehicle = new Vehicle();

    // Generate percentage to choose if to equip with etc
    double per;
    if(isCar) per = uniform(0, 1, CAR_RNG);
    else per = uniform(0, 1, TRUCK_RNG);

    bool hasEtc = (per < p);
    // Set hasEtc field
    vehicle->setHasEtc(hasEtc);

    // Set isCar field
    vehicle->setIsCar(isCar);

    // Set vehicle name for identification in Qtenv
    std::string name = (isCar ? "Car" : "Truck");
    name += (hasEtc ? "_ETC" : "_Std");
    vehicle->setName(name.c_str());

    // Message kind determines the color in Qtenv animation:
    // 0=red (Std Car), 1=green (ETC Car), 4=yellow (Std Truck), 5=cyan (ETC Truck)
    if (isCar && hasEtc)
        vehicle->setKind(1);   // Green: Car with ETC
    else if (isCar && !hasEtc)
        vehicle->setKind(0);   // Red: Car without ETC
    else if (!isCar && hasEtc)
        vehicle->setKind(5);   // Cyan: Truck with ETC
    else
        vehicle->setKind(4);   // Yellow: Truck without ETC

    // Send vehicle out to barrier
    send(vehicle, "out");

    EV_INFO << "Highway::generateVehicle - " << (isCar ? "Car" : "Truck") << " " << (vehicle->getHasEtc() ? "with ETC" : "without ETC") << endl;
}

void Highway::finish(){

    cSimpleModule::finish();
}


Highway::~Highway(){

    // Cancel scheduled timer
    cancelEvent(timerCar_);

    // Delete timer
    delete timerCar_;

    EV_INFO << "Highway::~Highway - car timer deleted" << endl;

    // Cancel scheduled timer
    cancelEvent(timerTruck_);

    // Delete timer
    delete timerTruck_;

    EV_INFO << "Highway::~Highway - truck timer deleted" << endl;

}
