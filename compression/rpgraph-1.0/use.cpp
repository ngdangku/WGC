/* use.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.
   Copyright (C) 2020, Dengke Wu.

   use -- uses the RepairGraph in an interactive console

   Additionally accept the third argument. "random" and "sequence" to do random
   access test and sequential access test, respectively.

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
#include <cstring>
#include <random>
#include <chrono>
#include <ctime>
#include "RepairGraph.h"

using namespace std;

const int TIMES = 100000;

int random_access_test(RepairGraph *g){
    if(g == nullptr) return 1;
    int n = g->nodes;
    int e = g->edges;

    default_random_engine generator;
    generator.seed(time(nullptr));
    uniform_int_distribution<int> distribution(0,n-1);

    int cnt = 0;
    long long edges = 0;
    auto t1 = clock();
    while(cnt<TIMES){
        int node = distribution(generator) + 1;
        uint * adj = g->adj(node);
        uint outd = g->outdegree(node);
        if(outd>0) {
            for(uint i=0;i<adj[0];i++) {
                edges=(edges+1)%(e+1);
            }
        }
        cnt++;
    }
    cout << "Random accessing" << endl;
    double avg = (1.0*clock()-t1)/(double)CLOCKS_PER_SEC*1e9/edges;
    printf("time: %fns/edge\n",avg);
    return 0;
}

int sequential_access_test(RepairGraph *g){
    if(g == nullptr) return 1;
    int n = g->nodes;
    int e = g->edges;
    cout << n << " " << e << endl;

    default_random_engine generator;
    generator.seed(time(nullptr));
    uniform_int_distribution<int> distribution(0,n-1);

    int start_node = distribution(generator); // [0,n)

    int cnt = 0;
    long long edges = 0;
    auto t1 = clock();
    while(cnt<TIMES){
        int node = (cnt+start_node)%n + 1;
        uint * adj = g->adj(node);
        uint outd = g->outdegree(node);
        if(outd>0) {
            for(uint i=0;i<adj[0];i++) {
                edges=(edges+1)%(e+1);
            }
        }
        cnt++;
    }
    cout << "Sequential accessing" << endl;
    double avg = (1.0*clock()-t1)/(double)CLOCKS_PER_SEC*1e9/edges;
    printf("time: %fns/edge\n",avg);
    return 0;
}

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


    if(argc >= 3){
        if(strcmp(argv[2], "random") == 0){
            cout << "Random access test" << endl;
            return random_access_test(g);
        }else if(strcmp(argv[2] , "sequence") == 0){
            cout << "Sequence access test" << endl;
            return sequential_access_test(g);
        }else{
            cout << "Argument not recognised" << endl;
        }
    }


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
