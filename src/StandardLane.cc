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

#include "StandardLane.h"

Define_Module(StandardLane);

void StandardLane::initialize()
{
    mean = par("mean");
    stddev = par("stddev");
    useExponentialDistribution = par("useExponentialDistribution");

    Lane::initialize();
}

simtime_t StandardLane::getServiceTime(bool isCar){
    // Use third (index = 2) physical rng

    if(useExponentialDistribution){
        simtime_t serviceTime = exponential(mean, SERVICE_RNG);
        if(isCar)
            return serviceTime;
        else
            return serviceTime * 1.5;
    }


    else{
        simtime_t serviceTime = lognormal(mean, stddev, SERVICE_RNG);
        if(isCar)
            return serviceTime;
        else
            return serviceTime * 1.5;
    }
}
