
#include <iostream>
#include <cassert>
#include <vector>
#include "basic.h"
#include "RepairGraph.h"

using namespace std;

#ifndef ullong
#define ullong unsigned long long
#endif

/* Time meassuring */
double ticks= (double)sysconf(_SC_CLK_TCK);
struct tms t1,t2;

void start_clock() {
    times (&t1);
}


double stop_clock() {
    times (&t2);
    return (t2.tms_utime-t1.tms_utime)/ticks;
}


/* end Time meassuring */

int main(int argc, char ** argv) {
    if(argc<3) {
        cout << "Usage: " << argv[0] << " <graph> <nodes list>" << endl;
        return 0;
    }
    char * graph_file = argv[1];
    char * list_file = argv[2];

    RepairGraph * gr = new RepairGraph(graph_file);

    FILE * list_fp = fopen64(list_file,"r");
    assert(list_fp!=NULL);
    uint queries;
    fread(&queries, sizeof(uint), 1, list_fp);
    ullong recovered = 0;
    double t = 0;
    for(uint i=0;i<queries;i++) {
        uint q;
        fread(&q,sizeof(uint),1,list_fp); q++;
        start_clock();
        uint * l = gr->adj(q);
        t += stop_clock();
        recovered += l[0];
        //free(l);
    }
    t *= 1000;                   // to milliseconds
    fclose(list_fp);
    cout << "Graph size: " << gr->size() << endl;
    cout << "Bits per link: " << 1.*gr->size()/gr->e*8 << endl;
    cout << "Recovered Nodes: " << recovered << endl;
    cout << "Queries: " << queries << endl;
    cout << "Total time(ms): " << t << endl;
    cout << "Time per query: " << t/queries << endl;
    cout << "Time per link: " << t/recovered << endl;
    delete gr;
}
