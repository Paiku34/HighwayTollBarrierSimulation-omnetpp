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

#ifndef __HIGHWAYBARRIER_HIGHWAY_H_
#define __HIGHWAYBARRIER_HIGHWAY_H_

#define CAR_RNG 0
#define TRUCK_RNG 1

#include <omnetpp.h>
#include "Vehicle_m.h"

using namespace omnetpp;

class Highway : public cSimpleModule
{
    double p;
    double Tc;
    double Tt;
    cMessage* timerCar_;
    cMessage* timerTruck_;

protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    void scheduleTimer(bool isCar);
    void generateVehicle(bool isCar);
    virtual void finish() override;
    ~Highway();
};

#endif
