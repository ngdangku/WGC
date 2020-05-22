/* RePair.h
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   RePair

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

#include "RePair.h"
#include "util.h"

RePair::RePair(uint * buff, uint datalen, uint _bufflen, char * _basename, uint _K) {
    data = buff;
    m = datalen;
    n = datalen;
    heap = NULL;
    bufflen = _bufflen;
    int * data_tmp = (int*)buff;
    mini = data_tmp[0];
    int maxi = data_tmp[0];
    for(uint i=1;i<m;i++) {
        if(mini>data_tmp[i]) mini=data_tmp[i];
        if(maxi<data_tmp[i]) maxi=data_tmp[i];
    }
    mini--;
    for(uint i=0;i<datalen;i++)
        data[i]=data[i]-mini;
    max_assigned = maxi-mini;
    max_value = max_assigned;
    K=_K;
    lastpos=0;
    basename = string(_basename);
    string fpname = basename + ".dict";
    fpDict = fopen64(fpname.data(),"w");
}


RePair::~RePair() {
    delete [] data;
}


void RePair::fillHeap() {
    if(heap!=NULL) {
        delete heap;
        heap = NULL;
    }
    heap = new Heap(K);
    assert(heap!=NULL);

    uint hashsize = (bufflen-m-1)/2;
    hpos = new HashTablePos((uint*)data,m,(int*)(data+m+1),hashsize);
    assert(hpos!=NULL);

    lastpos %= m;
    uint blastpos = lastpos;
    bool save=true;
    uint pos=lastpos;

    do {
        if(!hpos->increment(pos)) {
            if(save) {
                save = false;
                lastpos = pos;
            }
        }
        pos = (pos+1)%m;
    } while(pos!=blastpos);

    int * hasharray = (int*)(data+m+1);
    for(uint i=0;i<hashsize;i++)
        if(hasharray[2*i]>=0)
            heap->insert(new pair<uint,uint>((uint)hasharray[2*i],(uint)hasharray[2*i+1]));
    delete hpos;
}


uint RePair::replacePairs() {
    fillHeap();
    cout << "\t* pairs counted" << endl;
    uint c = 0;
    uint countR = 0;
    uint hashsize = (bufflen-m-1)/2;

    for(uint i=1;i<heap->inserted;i++)
        if(heap->elements[i]!=NULL && heap->elements[i]->second>1)
            c++;

    if(c==0) {
        delete heap;
        heap = NULL;
        return 0;
    }

    uint _c = c;

    pair<uint,uint> ** ret = new pair<uint,uint>*[c];
    hpairs = new HashTablePairs((int*)(data+m+1),2*hashsize/3);
    for(uint i=1;i<heap->inserted;i++)
    if(heap->elements[i]!=NULL && heap->elements[i]->second>1) {
        uint pos = heap->elements[i]->first;
        ret[--c] = new pair<uint,uint>(pos,NOT_SEEN);
        hpairs->insert(data[pos],data[pos+1],c);
    }

    for(uint i=0;i<m;i++) {
        c = hpairs->get(data[i],data[i+1]);
        if(c!=0) {
            c--;
            switch(ret[c]->second) {
                case NOT_SEEN: ret[c]->first = i;
                ret[c]->second = FIRST;
                break;
                case FIRST:     if(data[ret[c]->first]==data[i] && data[ret[c]->first+1]==data[i+1]) {
                    if(i==ret[c]->first+1) break;
                    addRule(++max_assigned,data[i],data[i+1]);
                    ret[c]->second = max_assigned;
                    data[ret[c]->first] = ret[c]->second;
                    data[ret[c]->first+1] = 0;
                    countR++;
                }
                else {
                    ret[c]->first=i;
                    break;
                }
                default:            data[i] = ret[c]->second;
                data[i+1] = 0;
            }
        }
    }
    cout << "\t* pairs replaced" << endl;

    rearrange();

    cout << "\t* sequence rearranged" << endl;

    delete heap;
    heap=NULL;
    for(uint i=0;i<_c;i++)
        delete ret[i];
    delete [] ret;
    delete hpairs;

    return countR;
}


void RePair::rearrange() {
    uint rptr=0;
    for(uint i=0;i<m;i++) {
        if(data[i] != 0) {
            data[rptr] = data[i];
            rptr++;
        }
    }
    data[rptr] = 0;
    m = rptr;
}


void RePair::addRule(uint nvalue, uint p1, uint p2) {
    my_fwrite(&nvalue,sizeof(uint),1,fpDict);
    my_fwrite(&p1,sizeof(uint),1,fpDict);
    my_fwrite(&p2,sizeof(uint),1,fpDict);
}


void RePair::compress(uint nodes) {
    uint i = 1; uint lasti=1;
    while(true) {
        cout << "Iteration " << i++ << ":" << endl;
        while(replacePairs()>0) {
            cout << "\t* |C|/|T|: " << 1.*m/n << endl;
            //cout.setw(2);
            cout << "Iteration " << i++ << ":" << endl;
        }
        if(lasti<=i) break;
        uint sorted=0;
        cout << "Reordering symbols: ";
        cout.flush();
        for(uint j=0;j<m;j++) {
            if(data[j]<=nodes) {
                for(uint k=j+1;k<=m;k++)
                if(data[k]<=nodes) {
                    sort(data+j+1,data+k);
                    sorted++;
                    break;
                }
            }
        }
        cout << sorted << endl;
        lasti = i;
    }
}


bool RePair::save(uint nodes) {
    return saveRPFile(nodes);
}


bool RePair::saveRPFile(uint nodes) {
    nodes++;
    string fname = basename + ".rp";
    FILE * fp = fopen64(fname.data(),"w");
    if(fp==NULL) {
        cerr << "Error opening output file: " << fname << endl;
        exit(-5);
    }
    my_fwrite(&mini,sizeof(uint),1,fp);
    my_fwrite(&max_value,sizeof(uint),1,fp);
    my_fwrite(&max_assigned,sizeof(uint),1,fp);
    my_fwrite(&m,sizeof(uint),1,fp);
    my_fwrite(&n,sizeof(uint),1,fp);
    my_fwrite(&nodes,sizeof(uint),1,fp);
    uint writ = 0;
    for(uint i=0;i<m;i++) {
        if(data[i]>nodes) {
            data[i]-=(nodes);
            my_fwrite(&data[i],sizeof(uint),1,fp);
            writ++;
        }
    }
    my_fwrite(&data[m],sizeof(uint),1,fp);
    fseeko64(fp,3*sizeof(uint),SEEK_SET);
    my_fwrite(&writ,sizeof(uint),1,fp);
    n -= (m-writ);
    my_fwrite(&n,sizeof(uint),1,fp);
    fclose(fp);
    return true;
}
