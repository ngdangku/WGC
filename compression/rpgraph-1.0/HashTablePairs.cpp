/* HashTablePairs.cpp
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   HashTablePairs

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

#include "HashTablePairs.h"

HashTablePairs::HashTablePairs(int * buff, uint buffLen) {
    this->buff=buff;
    this->buffLen=buffLen;
    for(uint i=0;i<3*buffLen;i++)
        buff[i] = -1;
    inserted=collisions=0;
    queries=0;
}


HashTablePairs::~HashTablePairs() {
}


uint HashTablePairs::get(uint p1, uint p2) {
    int p = h(p1,p2);
    uint lcollisions=0;
    while(buff[3*p]!=-1) {
        if(lcollisions>=buffLen*maxAlpha)
            return 0;
        if((int)p1==buff[3*p] && (int)p2==buff[3*p+1]) {
            return buff[3*p+2]+1;
        }
        lcollisions++;
        p = (p+s(p1,p2))%buffLen;
    }
    return 0;
}


bool HashTablePairs::insert(uint p1, uint p2, uint value) {
    queries++;
    int p = h(p1,p2);
    uint lcollisions=0;
    while(buff[3*p]!=-1) {
        if(lcollisions>=buffLen*maxAlpha)
            return false;
        if((int)p1==buff[3*p] && (int)p2==buff[3*p+1]) {
            buff[3*p+2] = value;
            return true;
        }
        collisions++; lcollisions++;
        p = (p+s(p1,p2))%buffLen;
    }
    if((double)inserted/buffLen>=maxAlpha)
        return false;
    //printf("Inserted: %d\tAlpha: %f\n",inserted,(double)inserted/buffLen);
    buff[3*p] = p1;
    buff[3*p+1] = p2;
    buff[3*p+2] = value;
    inserted++;
    return true;
}


uint HashTablePairs::h(uint p1, uint p2) {
    return ((p1+(p1<<16))^p2)%buffLen;
    //return ((p1*0x1f1f1f1f)^p2)%buffLen; // from http://stackoverflow.com/questions/682438/hash-function-providing-unique-uint-from-an-integer-coordinate-pair
}


uint HashTablePairs::s(uint p1, uint p2) {
    return 1;
}


/*
uint HashTablePairs::h(uint p1, uint p2) {
    //return (p1+(p2<<12))%buffLen;
  return (p1+((p1<<16)^p2))%buffLen;
}

#include <iostream>
using namespace std;

uint HashTablePairs::s(uint p1, uint p2) {
    return max(1u,(p1^p2)%buffLen);
}*/
