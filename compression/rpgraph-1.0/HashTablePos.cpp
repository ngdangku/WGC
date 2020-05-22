/* HashTablePos.cpp
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   HashTablePos

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

#include "HashTablePos.h"

HashTablePos::HashTablePos(uint * data, uint dataLen, int * buff, uint buffLen) {
    this->data=data;
    this->dataLen=dataLen;
    this->buff=buff;
    this->buffLen=buffLen;
    for(uint i=0;i<2*buffLen;i++)
        buff[i] = -1;
    inserted=collisions=0;
    queries=0;
}


HashTablePos::~HashTablePos() {
}


bool HashTablePos::increment(uint pos) {
    queries++;
    int p = h(pos);
    uint lcollisions=0;
    while(buff[2*p]!=-1) {
        if(lcollisions>=buffLen*maxAlpha) {
            return false;
        }
        if(cmp(pos,buff[2*p])) {
            buff[2*p+1]++;
            return true;
        }
        collisions++; lcollisions++;
        #ifdef VERBOSE
        //if(lcollisions%10000==0) {
        //	printf("collisions: %u\n",lcollisions); fflush(stdout);
        //}
        #endif
        p = (p+s(pos))%buffLen;
    }
    if((double)inserted/buffLen>=maxAlpha) {
        return false;
    }
    //printf("Inserted: %d\tAlpha: %f\n",inserted,(double)inserted/buffLen);
    buff[2*p] = pos;
    buff[2*p+1] = 1;
    inserted++;
    return true;
}


uint HashTablePos::get(uint pos) {
    int p = h(pos);
    uint lcollisions=0;
    while(buff[2*p]!=-1) {
        if(lcollisions>=buffLen*maxAlpha)
            return 0;
        if(cmp(pos,buff[2*p])) {
            return buff[2*p+1]+1;
        }
        lcollisions++;
        p = (p+s(pos))%buffLen;
    }
    return 0;
}


bool HashTablePos::insert(uint pos, uint value) {
    int p = h(pos);
    uint lcollisions=0;
    while(buff[2*p]!=-1) {
        if(lcollisions>=buffLen*maxAlpha)
            return false;
        if(cmp(pos,buff[2*p])) {
            buff[2*p+1] = value;
            return true;
        }
        collisions++; lcollisions++;
        p = (p+s(pos))%buffLen;
    }
    if((double)inserted/buffLen>=maxAlpha)
        return false;
    //printf("Inserted: %d\tAlpha: %f\n",inserted,(double)inserted/buffLen);
    buff[2*p] = pos;
    buff[2*p+1] = value;
    inserted++;
    return true;
}


uint HashTablePos::h(uint pos) {
    return ((data[pos]+(data[pos]<<16))^data[pos+1])%buffLen;
    //return ((data[pos]*0x1f1f1f1f)^data[pos+1])%buffLen; // from http://stackoverflow.com/questions/682438/hash-function-providing-unique-uint-from-an-integer-coordinate-pair
}


uint HashTablePos::s(uint pos) {
    return data[pos];
}


/*
uint HashTablePos::h(uint pos) {
    //return (data[pos]+(data[pos+1]<<12))%buffLen;
  return (data[pos]+((data[pos]<<16)^data[pos+1]))%buffLen;
  //return ((data[pos]+(data[pos]<<16))^data[pos+1])%buffLen;
}

#include <iostream>
using namespace std;
uint HashTablePos::s(uint pos) {
    return max(1u,(data[pos]^data[pos+1])%buffLen);
}*/

bool HashTablePos::cmp(uint pos1, uint pos2) {
    return data[pos1]==data[pos2] && data[pos1+1]==data[pos2+1];
}
