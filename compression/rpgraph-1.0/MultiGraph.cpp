/*  MultiGraph.cpp
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

#include "MultiGraph.h"
#include <sstream>
#include <fstream>
/*
    uint parts;
    RepairGraph ** graphs;
    uint * offset;
*/

string tostr(uint i) {
    stringstream ss;
    ss << i;
    return string(ss.str());
}


MultiGraph::MultiGraph(string base_name, int parts) {
    offset = new uint[parts+1];
    string fname = base_name+string(".offset");
    ifstream input(fname.c_str());
    for(uint i=0;i<(uint)parts+1;i++) {
        input.read((char*)(&(offset[i])),sizeof(uint));
        //cout << "offset[" << i << "]=" << offset[i] << endl;
    }
    input.close();
    graphs = new RepairGraph*[parts];
    for(uint i=0;i<(uint)parts;i++) {
        fname = base_name+tostr(i+1);
        graphs[i] = new RepairGraph((char*)fname.c_str());
    }
    this->parts=(uint)parts;
    this->nodes = offset[(uint)parts]-1;
    this->edges = 0;
    for(uint i=0;i<(uint)parts;i++) {
        this->edges += (uint64)graphs[i]->edges;
        //cout << " " << i << "- edges=" << edges << endl;
    }
    /*cout << "Graph loaded" << endl;
    cout << " number of nodes: " << nodes << endl;
    cout << " number of edges: " << edges << endl;
    cout << " size in bytes  : " << size() << endl;
    cout << " bits per edge  : " << 8.*size()/edges << endl;*/
}


MultiGraph::~MultiGraph() {
    for(uint i=0;i<parts;i++)
        delete graphs[i];
    delete [] graphs;
    delete [] offset;
}


uint MultiGraph::numNodes() {
    return nodes;
}


uint64 MultiGraph::numEdges() {
    return edges;
}


uint * MultiGraph::adj(uint i) {
    if(i>=offset[parts]) return NULL;
    uint ind = 0;
    while(offset[ind+1]<=i) ind++;
    return graphs[ind]->adj(i-offset[ind]+1);
}


uint MultiGraph::outdegree(uint i) {
    if(i>=offset[parts]) return 0;
    uint ind = 0;
    while(offset[ind+1]<=i) ind++;
    return graphs[ind]->outdegree(i-offset[ind]+1);
}


ulong MultiGraph::size() {
    ulong ret = sizeof(MultiGraph)+sizeof(uint)*(parts+1);
    for(uint i=0;i<parts;i++)
        ret += graphs[i]->size();
    return ret;
}
