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

package highwaybarrier.simulations;
import highwaybarrier.Highway;
import highwaybarrier.Barrier;

network HighwayBarrier
{
    @display("bgb=500,250;bgi=background/streetmap,stretch");
    submodules:
        highway: Highway {
            @display("p=120,125;i=block/departure,gold;is=l;tt=Generates cars and trucks with exponential interarrival times");
        }
        barrier: Barrier {
            @display("p=370,125;i=block/network2,steelblue;is=l;tt=Highway Toll Barrier with ETC and Standard lanes");
        }
    connections:
        highway.out --> { @display("ls=#f0c040,3"); } --> barrier.in;
}
