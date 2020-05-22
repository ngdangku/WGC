#include "basic.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <algorithm>
#include "MultiGraph.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc<3) {
        cout << "Usage: " << argv[0] << " <base_name> <parts>" << endl;
        return 0;
    }
    uint max_outdegree = 0;
    char * fname = argv[1];
    char * fout = new char[strlen(argv[1])+20];
    sprintf(fout,"%s.rebuilt",fname);
    FILE * fp = fopen64(fout,"w");
    assert(fp!=NULL);
    uint parts = 0;
    {
        stringstream ss(argv[2]);
        ss >> parts;
    }
    MultiGraph * g = new MultiGraph(string(fname),parts);
    uint nodes = g->numNodes();
    uint64 edges = g->numEdges();
    cout << "Nodes: " << nodes << endl;
    cout << "Edges: " << edges << endl;
    cout << "Graph size: " << g->size() << endl;
    cout << "Bits per link: " << 1.*g->size()/edges*8 << endl;
    fwrite(&nodes,sizeof(uint),1,fp);
    fwrite(&edges,sizeof(uint64),1,fp);
    for(uint i=1;i<=nodes;i++) {
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
        edges -= adj[0];
    }
    cout << "Max outdegree: " << max_outdegree << endl;
    cout << "Not written: " << edges << endl;

    fclose(fp);
    delete [] fout;
    delete g;
    return 0;
}
