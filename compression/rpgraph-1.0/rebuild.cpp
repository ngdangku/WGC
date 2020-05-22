/* rebuild.h
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   rebuild

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
#include <cassert>
#include <algorithm>
#include "RepairGraph.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc<2) {
        cout << "Usage: " << argv[0] << " <base_name>" << endl;
        return 0;
    }
    uint max_outdegree = 0;
    char * fname = argv[1];
    char * fout = new char[strlen(argv[1])+20];
    sprintf(fout,"%s.rebuilt",fname);
    FILE * fp = fopen64(fout,"w");
    assert(fp!=NULL);
    RepairGraph * g = new RepairGraph(fname);
    cout << "Nodes: " << g->nodes << endl;
    cout << "Edges: " << g->edges << endl;
    cout << "Graph size: " << g->size() << endl;
    cout << "Bits per link: " << 1.*g->size()/g->edges*8 << endl;
    fwrite(&g->nodes,sizeof(int),1,fp);
    uint64 edges_tmp = g->edges;
    fwrite((void*)&edges_tmp,sizeof(uint64),1,fp);
    for(uint i=1;i<=g->nodes;i++) {
        uint * adj = g->adj(i);
        uint outd = g->outdegree(i);
        sort(adj+1,adj+adj[0]+1);
        int w = -i;
        fwrite(&w,sizeof(int),1,fp);
        for(uint i=1;i<=adj[0];i++)
            fwrite(&adj[i],sizeof(int),1,fp);
        if(adj[0]>max_outdegree)
            max_outdegree=adj[0];
        if(adj[0]!=outd) {
            cout << "Error, el outdegree no coincide!!!" << endl;
            break;
        }
    }
    cout << "Max outdegree: " << max_outdegree << endl;

    fclose(fp);
    delete [] fout;
    delete g;
    return 0;
}
