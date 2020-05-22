/* build_index.cpp
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   build_index

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
#include "basic.h"
#include "bitrankw32int.h"
#include "RePair.h"
#include "util.h"

using namespace std;

int main(int argc, char ** argv) {
    if(argc!=4) {
        cout << "Usage: " << argv[0] << " <GRAPH> <MEM> <K>" << endl;
        return 0;
    }
    char * fname = new char[strlen(argv[1])+20];

    FILE * fp = fopen64(argv[1],"r");
    if(fp==NULL) {
        cout << "Error opening input file: " << argv[1] << endl;
        return -2;
    }

    uint rd = 0;

    uint mem = atoi(argv[2]);
    uint K = atoi(argv[3]);

    uint nodes; uint64 edges;
    rd += fread(&nodes,sizeof(nodes),1,fp);
    rd += fread(&edges,sizeof(edges),1,fp);

    /*cout << "Nodes: " << nodes << endl;
    cout << "Edges: " << edges << endl;
    cout << " bytes used to represent the number of edges: " << sizeof(edges) << endl;*/

    if(nodes+edges > (2800000000u/4)) {
        cout << "The graph is too big, you should cut it first." << endl;
        return -3;
    }

    if(mem < (nodes+edges)*1.01) {
        cout << "The memory assigned should be at least 1% on top of the size of the graph." << endl;
        cout << "In this case, mem >= " << (uint)((nodes+edges)*1.01+1) << endl;
        return -2;
    }

    int * text = new int[mem];   // deleted in RePair.cpp
    for(uint i=0;i<mem;i++)
        text[i] = 0;
    uint * offsets = new uint[nodes+1];

    if(text==NULL || offsets==NULL) {
        cout << "Not enough memory for creating the buffer." << endl;
        return -1;
    }

    uint ml=0,al=0;
    int min = 0; uint nodes_read=0;
    bool fst = false; int last=0;
    for(uint i=0;i<nodes+edges;i++) {
        int k;
        rd += fread(&k,sizeof(k),1,fp);
        if(k<0) {
            //bitset(bitmap,i);
            ml = max(al,ml);
            al = 0;
            offsets[-k-1]=i+k+1;
            if(k<min) min=k;
            nodes_read++;
            text[i]=k;
            fst = true;
        }
        else {
            if(fst) {
                text[i]=k;
                last = k;
                fst = false;
            }
            else {
                text[i] = k-last;
                last = k;
            }
            al++;
        }
    }
    ml = max(al,ml);
    min--;
    text[nodes+edges]=0;
    offsets[nodes]=edges;
    for(uint i=0;i<nodes+edges;i++)
        text[i] -= min;
    //bitset(bitmap,nodes+edges);

    /*cout << "rd=" << rd << endl;
    cout << "nodes+edges=" << nodes+edges << endl;
    cout << "nodes_read=" << nodes_read << endl;*/

    assert(nodes_read == nodes);
    assert(rd==(nodes+edges+2));

    uint plen = bits(edges);
    uint offsets_len = (nodes+1)*plen/W+1;
    uint * off = new uint[offsets_len];
    if(off==NULL) {
        cout << "Not enough memory for compacting the offsets." << endl;
        return -1;
    }

    for(uint i=0;i<=nodes;i++)
        SetField(off,plen,i,offsets[i]);
    delete [] offsets;
    offsets=off;

    cout << "Nodes: " << nodes << endl;
    cout << "Edges: " << edges << endl;
    cout << "Nodes read: " << nodes_read << endl;

    sprintf(fname,"%s",argv[1]);
    RePair * rc = new RePair((uint*)text,(edges+nodes),mem,fname,K);
    cout << "Graph read, starting compression." << endl;
    rc->compress(nodes);
    cout << "Compression finished." << endl;
    cout << "Saving files." << endl;
    rc->save(nodes);

    sprintf(fname,"%s.ptr",argv[1]);
    FILE * f_out = fopen64(fname,"w");
    my_fwrite(&min,sizeof(int),1,f_out);
    my_fwrite(&nodes,sizeof(uint),1,f_out);
    my_fwrite(&edges,sizeof(uint),1,f_out);
    my_fwrite(&ml,sizeof(uint),1,f_out);
    my_fwrite(offsets,sizeof(uint),offsets_len,f_out);
    fclose(f_out);

    cout << "Files saved." << endl;
    delete rc;
    delete [] fname;
    delete [] offsets;
    return 0;
}
