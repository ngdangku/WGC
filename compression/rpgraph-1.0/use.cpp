/* use.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   use -- uses the RepairGraph in an interactive console

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include "RepairGraph.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc<2) {
        cout << "Usage: " << argv[0] << " <base_name>" << endl;
        return 0;
    }
    char * fname = argv[1];
    RepairGraph * g = new RepairGraph(fname);
    cout << "Graph size: " << g->size() << endl;
    cout << "Bits per link: " << 1.*g->size()/g->e*8 << endl;
    cout << "Insert node ids to query adjacency lists, 0 ends the console" << endl;
    while(true) {
        int node;
        cout << "> ";
        cin >> node;
        if(node==0) break;
        uint * adj = g->adj(node);
        uint outd = g->outdegree(node);
        cout << "Outdegree: " << outd << endl;
        cout << "Adjacency list: ";
        if(outd>0)
        for(uint i=0;i<adj[0];i++) {
            if(i%5==0) cout << endl;
            printf(" %11d",adj[i+1]);
        }
        cout << endl;
        //free(adj);
    }
    delete g;
    return 0;
}
