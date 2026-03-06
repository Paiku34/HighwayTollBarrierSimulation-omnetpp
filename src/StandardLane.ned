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

simple StandardLane extends Lane
{
    parameters:
        double mean @unit("s") = default(1s);
        double stddev @unit("s") = default(1s);
        
        bool useExponentialDistribution = default(true);
        @display("i=block/activeq,#e67e22;q=queue;tt=Standard toll lane - manual payment");
    @class(StandardLane);
}
