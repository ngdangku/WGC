/*  MultiGraph.h
 * Copyright (C) 2010, Francisco Claude, all rights reserved.
 *
 * Francisco Claude <fclaude@cs.uwaterloo.ca>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MULTI_GRAPH_H
#define MULTI_GRAPH_H

#include "basic.h"
#include "RepairGraph.h"
#include <iostream>

using namespace std;

class MultiGraph
{
    public:
        MultiGraph(string base_name, int parts);
        ~MultiGraph();

        uint * adj(uint i);
        uint outdegree(uint i);
        ulong size();
        uint numNodes();
        uint64 numEdges();

    protected:
        uint parts;
        RepairGraph ** graphs;
        uint * offset;
        uint64 edges;
        uint nodes;

};
#endif
