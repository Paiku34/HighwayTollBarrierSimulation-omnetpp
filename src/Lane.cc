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

#include "Lane.h"
#include "Vehicle_m.h"

Define_Module(Lane);

void Lane::initialize()
{
    // Register signals
    TotalCars_Signal = registerSignal("TotalCars");
    CarsInQueue_Signal = registerSignal("CarsInQueue");
    ResponseTime_Signal = registerSignal("ResponseTime");
    WaitingTime_Signal = registerSignal("WaitingTime");

    sigVector.setName("Cars in queue");

    vehicleQueue = new cQueue();

    timer_ = new cMessage("timer");

    // Starting value for signal
    emit(TotalCars_Signal, 0);
    emit(CarsInQueue_Signal, 0);

    if(simTime() > getSimulation()->getWarmupPeriod())
        sigStats.collect(0);
}

void Lane::handleMessage(cMessage *msg)
{
    if(msg->isName("timer"))
        endService();

    else vehicleArrival(msg);
}

int Lane::getVehiclesQueued(){
    return vehicleQueue->getLength();
}

void Lane::vehicleArrival(cMessage *msg){
    // Cast to vehicle
    Vehicle* vehicle;
    vehicle = check_and_cast<Vehicle*>(msg);

    EV_INFO << "Lane::vehicleArrival - vehicle arrived (" << (vehicle->isCar() ? "Car" : "Truck") << ")" << endl;

    // Set the arrival time to the current simTime, to calculate waiting and response time
    vehicle->setArrivalTime(simTime());

    // Add to queue
    vehicleQueue->insert(vehicle);

    // Emit number of cars in the queue and in total
    emit(TotalCars_Signal, vehicleQueue->getLength());
    emit(CarsInQueue_Signal, vehicleQueue->getLength() - 1);

    if(simTime() > getSimulation()->getWarmupPeriod())
        sigStats.collect(vehicleQueue->getLength() - 1);

    // If vehicle is the first to arrive immediately start its service
    if(vehicleQueue->getLength() == 1)
        startService();

}

void Lane::startService(){

    EV_INFO << "Lane::startService - start service time: ";

    startServiceTime = simTime();

    // Get vehicle type
    Vehicle* vehicle;
    vehicle = check_and_cast<Vehicle*>(vehicleQueue->front());

    // Emit waiting time for current vehicle
    emit(WaitingTime_Signal, simTime() - vehicle->getArrivalTime());

    // Get service time (uses child classes)
    simtime_t serviceTime = getServiceTime(vehicle->isCar());

    // Start timer for service time
    scheduleAt(simTime() + serviceTime, timer_);

    EV_INFO << serviceTime << endl;
}

void Lane::endService(){

    // Get serviced vehicle from queue
    Vehicle* vehicle;
    vehicle = check_and_cast<Vehicle*>(vehicleQueue->pop());

    // Emit service time for current vehicle
    emit(ResponseTime_Signal, simTime() - vehicle->getArrivalTime());

    emit(TotalCars_Signal, vehicleQueue->getLength());

    // Serviced cars might be the only one present, so queue = 0
    if(vehicleQueue->getLength() > 0){
        emit(CarsInQueue_Signal, vehicleQueue->getLength() - 1);
        if(simTime() > getSimulation()->getWarmupPeriod())
            sigStats.collect(vehicleQueue->getLength() - 1);
    }
    else{
        emit(CarsInQueue_Signal, 0);
        if(simTime() > getSimulation()->getWarmupPeriod())
            sigStats.collect(0);
    }

    EV_INFO << "Lane::endService - service time ended, remaining cars: " << vehicleQueue->getLength() << endl;

    delete vehicle;

    // If queue is not empty service next car
    if(!vehicleQueue->isEmpty())
        startService();
}

Lane::~Lane(){

    // Empty queue
    while(!vehicleQueue->isEmpty()){
        cObject* v = vehicleQueue->pop();
        delete v;
    }

    // Delete queue
    delete vehicleQueue;

    // Unschedule and delete timer
    cancelEvent(timer_);
    delete timer_;
}

void Lane::finish(){
    // Compute statistics

    EV_INFO << "Remaining cars: " << vehicleQueue->getLength() << endl;
    EV_INFO << "Mean vehicles in queue: " << sigStats.getMean() << endl;

    cSimpleModule::finish();
}

