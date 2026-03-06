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

module Barrier
{
    parameters:
        int N;
        int K;
        @display("bgb=420,480;bgi=background/streetmap,stretch");

    gates:
        input in;

    submodules:
        // Dispatcher: routes incoming vehicles to the appropriate lane type
        switch: Switch {
            @display("p=80,240;i=block/dispatch,dodgerblue;is=l;tt=Routes vehicles to ETC or Standard lanes");
        }

        // ETC lanes (green): dedicated to vehicles with electronic toll collection
        etcLane[K]: ETCLane {
            @display("p=310,60,column,55");
        }

        // Standard lanes (orange): for manual toll payment
        stdLane[N-K]: StandardLane {
            @display("p=310,200,column,55");
        }

    connections:
        in --> { @display("ls=#607d9e,2"); } --> switch.in;

        for i=0..(K-1){
            switch.out_etc++ --> { @display("ls=#2ecc71,2"); } --> etcLane[i].in;
        }

        for i=0..(N-K-1){
            switch.out_std++ --> { @display("ls=#e67e22,2"); } --> stdLane[i].in;
        }
}
