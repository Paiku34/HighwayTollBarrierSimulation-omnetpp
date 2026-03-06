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

#ifndef __HIGHWAYBARRIER_ETCLANE_H_
#define __HIGHWAYBARRIER_ETCLANE_H_

#include <omnetpp.h>
#include "Lane.h"

using namespace omnetpp;

class ETCLane : public Lane
{
    double serviceTime;
protected:
    virtual void initialize() override;
    virtual simtime_t getServiceTime(bool isCar) override;
    virtual void finish() override {
        EV_INFO << "ETCLane::finish" << endl;
        Lane::finish();
        EV_INFO << endl;
    }
};

#endif
