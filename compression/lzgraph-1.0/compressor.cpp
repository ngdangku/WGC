/* main.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   Graph compressor -- using differences

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

#include "LempelZiv.h"
#include "BLArray.h"
#include "bitrankw32int.h"

int main(int argc, char ** argv) {
    if(argc!=2) {
        cout << "Usage: " << argv[0] << " <file>" << endl;
        return 0;
    }
    FILE * fp = fopen64(argv[1],"r");
    assert(fp!=NULL);

    uint nodes, edges, rd = 0;
    uint64 edges_tmp;
    rd += fread(&nodes, sizeof(uint), 1, fp);
    rd += fread(&edges_tmp, sizeof(uint64), 1, fp);

    if(edges_tmp > (1<<30)) {
        cout << "This file is too big :(" << endl;
        return -1;
    }
    edges = (uint)edges_tmp;

    uint n = edges+1;
    uint * text = new uint[n];
    uint * ptrs = new uint[nodes+1];
    uint pos=0,ppos=0,last=0;
    cout << "* nodes=" << nodes << endl;
    cout << "* edges=" << edges << endl;
    cout << "Building structure...";
    uint ml = 0, al = 0;
    for(uint i=0;i<n+nodes;i++) {
        int s;
        rd += fread(&s, sizeof(int), 1, fp);
        if(s<0) {
            ml = max(ml,al);
            al = 0;
            ptrs[ppos++]=pos;
            last = 0;
        }
        else {
            text[pos++] = (ulong)(s-last);
            last = s;
            al++;
        }
    }
    ml = max(al,ml);
    ptrs[nodes]=edges;
    text[n-1] = 0;
    fclose(fp);
    assert(rd==(edges+nodes+2));
    BLArray * s, *p; uint * bm;
    cout << "done" << endl;
    cout << "Compressing data...";
    uint phrases = compress(text, n, &s, &p, &bm, ptrs);
    BitRankW32Int * br = new BitRankW32Int(bm,n+1,true,20);
    cout << "done" << endl;
    cout << "Phrases generated: " << phrases << endl;

    uint max_v_ptrs = 0;
    for(uint i=0;i<=nodes;i++) {
        ptrs[i] = br->rank(ptrs[i]);
        max_v_ptrs = max(max_v_ptrs, ptrs[i]);
    }
    BLArray * cptr = new BLArray(nodes+1, max_v_ptrs);
    for(uint i=0;i<=nodes;i++)
        cptr->setField(i,ptrs[i]);

    cout << "Saving file...";
    char * save = new char[string(argv[1]).length()+25];
    sprintf(save, "%s.lzg", argv[1]);
    fp = fopen(save, "w");
    assert(fp!=NULL);
    uint wr = 0;
    wr += fwrite(&nodes,sizeof(uint),1,fp);
    wr += fwrite(&edges,sizeof(uint),1,fp);
    wr += fwrite(&ml,sizeof(uint),1,fp);
    bool err = s->save(fp);
    err &= p->save(fp);
    err &= cptr->save(fp);
    assert(err && wr==3);
    fclose(fp);
    cout << "done" << endl;
    return 0;
}
