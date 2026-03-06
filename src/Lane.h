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

#ifndef __HIGHWAYBARRIER_LANE_H_
#define __HIGHWAYBARRIER_LANE_H_

#define SERVICE_RNG 2

#include <omnetpp.h>

using namespace omnetpp;

class Lane : public cSimpleModule
{
    cQueue* vehicleQueue;
    cMessage* timer_;
    simtime_t startServiceTime;

    // Signals
    simsignal_t TotalCars_Signal;
    simsignal_t CarsInQueue_Signal;
    simsignal_t WaitingTime_Signal;
    simsignal_t ResponseTime_Signal;

    cOutVector sigVector;
    cStdDev sigStats;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void vehicleArrival(cMessage *msg);
    virtual void startService();
    virtual simtime_t getServiceTime(bool isCar) { return 0; };
    virtual void endService();
    virtual void finish() override;
    ~Lane();
public:
    virtual int getVehiclesQueued();
};

#endif
