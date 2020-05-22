/**rebuild.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   rebuilds a LZGraph to plain format

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
#include "LZGraph.h"
#include <algorithm>

using namespace std;

int main(int argc, char ** argv) {
    if(argc<2) {
        cout << "Usage: " << argv[0] << " <lzg file>" << endl;
        return 0;
    }
    uint max_outdegree = 0;
    char * fname = argv[1];
    char * fout = new char[string(argv[1]).length()+20];
    sprintf(fout,"%s.rebuilt",fname);
    FILE * fp = fopen64(fout,"w");
    assert(fp!=NULL);
    LZGraph * g = new LZGraph(fname);
    cout << "Nodes: " << g->n << endl;
    cout << "Edges: " << g->e << endl;
    cout << "Graph size: " << g->size() << endl;
    cout << "Bits per link: " << 1.*g->size()/g->e*8 << endl;
    fwrite(&g->n,sizeof(uint),1,fp);
    uint64 edges = g->e;
    fwrite(&edges,sizeof(uint64),1,fp);
    for(uint i=1;i<=g->n;i++) {
        uint * adj = g->adj(i);
        uint outd = g->outdegree(i);
        int w = -i;
        fwrite(&w,sizeof(int),1,fp);
        sort(adj+1,adj+adj[0]+1);
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
