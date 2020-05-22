/* LempelZiv.cpp
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   LempelZiv -- Memory consuming implementation

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

LZTrie::LZTrie(uint id) {
    this->id = id;
}


LZTrie::~LZTrie() {
    map<uint, LZTrie* >::iterator it = c.begin();
    for(;it!=c.end();it++)
        delete it->second;
}


void LZTrie::add(uint id, uint symbol) {
    c[symbol] = new LZTrie(id);
}


LZTrie * LZTrie::get(uint symbol) {
    if(c.count(symbol)==0) return NULL;
    return c[symbol];
}


uint LZTrie::getId() {
    return id;
}


LZPhrase::LZPhrase(uint symbol, uint parent) {
    this->symbol = symbol;
    this->parent = parent;
}


LZPhrase::~LZPhrase() { }

int compress(uint * data, uint n, BLArray ** symb, BLArray ** parent, uint ** br, uint * start) {
    uint act_id = 0;
    uint max_symb = 0;
    uint num_phrases = 1;
    list<LZPhrase* > phrases;
    (*br) = new uint[2+n/W];
    for(uint i=0;i<2+n/W;i++)
        (*br)[i]=0;
    bitset(*br,0);
    uint l=1;
    LZTrie * root = new LZTrie(act_id++);
    phrases.push_back(new LZPhrase(0,0));
    for(uint i=0;i<n;) {
        while(i==start[l]) l++;
        LZTrie * rec = root;
        while(i<n && i<start[l]-1) {
            LZTrie * next = rec->get(data[i]);
            if(next==NULL) break;
            rec=next;
            i++;
        }
        if(i!=start[l]-1) {
            rec->add(act_id, data[i]);
        }
        else {
            l++;
        }
        bitset(*br, i+1);
        phrases.push_back(new LZPhrase(data[i], rec->getId()));
        num_phrases++; i++; act_id++;
        max_symb = max(max_symb, data[i]);
    }
    *symb = new BLArray(num_phrases, max_symb);
    *parent = new BLArray(num_phrases, act_id);
    list<LZPhrase* >::iterator it = phrases.begin();
    for(uint i=0;it!=phrases.end(); it++,i++) {
        (*symb)->setField(i, (*it)->symbol);
        (*parent)->setField(i, (*it)->parent);
        #ifdef VDEBUG
        cout << "phrase " << i << endl;
        cout << "  symbol: " << (*it)->symbol << endl;
        cout << "  parent: " << (*it)->parent << endl;
        cout << endl;
        #endif
    }
    //cout <<  "l=" << l << endl;
    return num_phrases;
}


uint * decompress(BLArray * symb, BLArray * parent, uint phrases, uint n) {
    uint pos = 0;
    stack<uint> v;
    uint * ret = new uint[n];
    for(uint i=1;i<phrases;i++) {
        uint k = i;
        while(k!=0) {
            //cout << "("<<k<<","<<symb->getField(k)<<")"<<endl;
            v.push(symb->getField(k));
            k = parent->getField(k);
        }
        while(!v.empty()) {
            ret[pos++] = v.top();
            v.pop();
        }
    }
    return ret;
}
