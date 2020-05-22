/* LZGraph.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   LZGraph -- using differences

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

#include "LZGraph.h"

LZGraph::LZGraph(char * fname) {
    FILE * fp = fopen(fname,"r");
    uint r = 0;
    assert(fp!=NULL);
    r+=fread(&n,sizeof(uint),1,fp);
    r+=fread(&e,sizeof(uint),1,fp);
    r+=fread(&ml,sizeof(uint),1,fp);
    assert(r==3);
    s = new BLArray(fp);
    p = new BLArray(fp);
    ptr = new BLArray(fp);
    info = new uint[ml+3];
}


LZGraph::~LZGraph() {
    delete s;
    delete p;
    delete ptr;
    delete [] info;
}


uint * LZGraph::adj(uint n) {
    n--;
    uint i=ptr->getField(n);
    uint j=ptr->getField(n+1);
    if(i>=j) {
        info[0]=0;
        return info;
    }
    uint ph = j-1;
    uint ph_pos = ph;
    uint k = 1;
    while(ph>=i) {
        ulong v = p->getField(ph_pos);
        info[k++]=s->getField(ph_pos);
        if(v!=0) {
            ph_pos=v;
        }
        else {
            ph_pos=--ph;
        }
    }
    info[0]=k-1;
    for(uint k=info[0];k>=2;k--)
        info[k-1] += info[k];
    return info;
}


uint LZGraph::outdegree(uint n) {
    adj(n); return info[0];
}


uint LZGraph::size() {
    return p->size()+s->size()+ptr->size()+(3+ml)*sizeof(uint);
}
