#include <iostream>
#include <sstream>
#include "MultiGraph.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc!=3) {
        cout << "Usage: " << argv[0] << " <base_name> <parts>" << endl;
        return 0;
    }
    string fname = string(argv[1]);
    uint parts = 0;
    {
        stringstream ss(argv[2]);
        ss >> parts;
    }
    MultiGraph * g = new MultiGraph(fname,parts);
    cout << "Graph size: " << g->size() << endl;
    cout << "Bits per link: " << 1.*g->size()/g->numEdges()*8 << endl;
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
