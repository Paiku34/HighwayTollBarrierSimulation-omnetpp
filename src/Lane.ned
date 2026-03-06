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

package highwaybarrier;

simple Lane
{
    parameters:
        
        // Total number of cars in the lane (queue + in service)
        @signal [TotalCars];
        @statistic[TotalCars_Stat] (source="TotalCars"; record=mean, vector);
        
        // Total number of cars in the queue
        @signal [CarsInQueue];
        @statistic[CarsInQueue_Stat] (source="CarsInQueue"; record=mean, vector);
        
        // Waiting time for cars
        @signal [WaitingTime];
        @statistic[WaitingTime_Stat] (source="WaitingTime"; record=mean, vector);
        
        // Response time for cars
        @signal [ResponseTime];
        @statistic[ResponseTime_Stat] (source="ResponseTime"; record=mean, vector);
        @display("i=block/activeq,#6c7a89;q=queue;tt=FIFO queue lane");
        
    gates:
        input in;
}
