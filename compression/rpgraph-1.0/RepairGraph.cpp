/* RepairGraph.h
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   RepairGraph

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

#include "RepairGraph.h"
#include "util.h"

RepairGraph::RepairGraph(char * file) {
    int error=0;
    char * fname = new char[strlen(file)+50];

    sprintf(fname,"%s.rpgraph",file);
    FILE * fp = fopen64(fname,"r");
    if(fp==NULL) {
        cout << "Error opening " << file << endl;
        exit(-2);
    }
    assert(fp!=NULL);
    uint min_value=0, max_assigned=0;
    max_value=0;
    my_fread(&min_value,sizeof(uint),1,fp);
    my_fread(&max_value,sizeof(uint),1,fp);
    my_fread(&max_assigned,sizeof(uint),1,fp);
    my_fread(&nbits, sizeof(uint), 1, fp);
    my_fread(&m,sizeof(uint),1,fp);
    my_fread(&n,sizeof(uint),1,fp);
    symbols = new uint[(m/32*nbits+m%32*nbits/32+1)];
    uint read = 0;
    while((read+=fread(symbols+read,sizeof(uint),1,fp))!=(m/32*nbits+m%32*nbits/32+1));

    my_fread(&bits_sn,sizeof(uint),1,fp);
    my_fread(&symbols_new_len,sizeof(uint),1,fp);
    csymb_n = new uint[symbols_new_len*bits_sn/W+1];
    my_fread(csymb_n,sizeof(uint),symbols_new_len*bits_sn/W+1,fp);
    BRR = new BitRankW32Int(fp,&error);
    assert(error==0);

    my_fread(&nodes,sizeof(uint),1,fp);
    my_fread(&edges,sizeof(uint),1,fp);
    my_fread(&ml,sizeof(uint),1,fp);

    bptrs = new BitRankW32Int(fp,&error);
    assert(error==0);
    bnoz = new BitRankW32Int(fp,&error);
    assert(error==0);

    e=edges;
    info = (uint*)malloc(sizeof(uint)*(ml+3));
    delete [] fname;
}


RepairGraph::~RepairGraph() {
    delete [] symbols;
    delete [] csymb_n;
    delete BRR;
    delete bnoz;
    delete bptrs;
    free(info);
}


uint RepairGraph::size() {
    uint sum = 0;
                                 // pointers
    sum += bnoz->SpaceRequirementInBits()/8+bptrs->SpaceRequirementInBits()/8;
                                 // C
    sum += sizeof(uint)*(m/32*nbits+m%32*nbits/32+1);
                                 // R_B
    sum += BRR->SpaceRequirementInBits()/8;
                                 // R_S
    sum += (symbols_new_len*bits_sn/W+1)*sizeof(uint);
    sum += (3+ml)*sizeof(uint);
    return sum;
}


void RepairGraph::expand(uint v) {
    if(v <= max_value) {
        pos++;
        info[pos] = v+info[pos-1];
        return;
    }
    uint aux1 = v-max_value-1;
    assert(BRR->IsBitSet(aux1));
    uint aux2=1;
    uint rank_aux=aux1-BRR->rank(aux1)+1;
    while(aux2!=0) {
        if(BRR->IsBitSet(aux1))
            aux2++;
        else {
            aux2--;
                                 //symbols_new[rank_aux]);
            expand(GetField(csymb_n, bits_sn, rank_aux));
            rank_aux++;
        }
        aux1++;
    }
}


uint * RepairGraph::adj(int v) {
    if(!bnoz->IsBitSet(v)) {
        info[0] = 0;
        return info;
    }
    uint nd = bnoz->rank(v);
    uint i = bptrs->select(nd);
    uint j = bptrs->select(nd+1);
    pos = 0;
    info[0] = 0;
    for(uint k=i;k<j;k++)
        expand(GetField(symbols,nbits,k));
    info[0]=pos;
    return info;
}


uint RepairGraph::outdegree(int v) {
    adj(v);
    return info[0];
}
