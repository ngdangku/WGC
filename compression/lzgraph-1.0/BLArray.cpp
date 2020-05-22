/* BLArray.cpp
   Copyright (C) 2007, Rodrigo Gonzalez, Francisco Claude F, all rights reserved.

   Bit length array

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

#include "BLArray.h"

using namespace std;

BLArray::BLArray(FILE * fp) {
    bool l = load(fp);
    if(!l) {
        array = NULL;
        n = 0;
        maxValue = 0;
        blen = 0;
        arrayLength = 0;
    }
    assert(l==true);
}


BLArray::BLArray(uint _n, uint _maxValue) {
    n = _n;
    maxValue = _maxValue;
    blen = bits(maxValue);
    arrayLength = getLength(n,blen);
    array = new uint[arrayLength];
    #ifdef VDEBUG
    cout << "Creating BitLength array" << endl;
    cout << "n: " << n << endl;
    cout << "maxValue: " << maxValue << endl;
    cout << "blen: " << blen << endl;
    cout << "arrayLength: " << arrayLength << endl;
    #endif
}


BLArray::~BLArray() {
    delete [] array;
}


/*
inline ulong BLArray::getField(ulong index) {
  register ulong i=index*blen/BLArray::WORD_SIZE, j=index*blen-BLArray::WORD_SIZE*i, result;
  //register ulong i=(index/W)*len+((index%W)*len)/W, j=((index%W)*len)%W,result;
  if (j+blen <= BLArray::WORD_SIZE)
    result = (array[i] << (BLArray::WORD_SIZE-j-blen)) >> (BLArray::WORD_SIZE-blen);
  else {
    result = array[i] >> j;
    result = result | (array[i+1] << (BLArray::DWORD_SIZE-j-blen)) >> (BLArray::WORD_SIZE-blen);
  }
  return result;
}

inline void BLArray::setField(ulong index, ulong x) {
   ulong i=index*blen/BLArray::WORD_SIZE, j=index*blen-i*BLArray::WORD_SIZE;
   //register ulong i=(index/W)*len+((index%W)*len)/W, j=((index%W)*len)%W;
   ulong mask = ((j+blen) < BLArray::WORD_SIZE ? ~0u << (j+blen) : 0) | ((BLArray::WORD_SIZE-j) < BLArray::WORD_SIZE ? ~0u >> (BLArray::WORD_SIZE-j) : 0);
   array[i] = (array[i] & mask) | x << j;
   if (j+blen>BLArray::WORD_SIZE) {
      mask = ((~0u) << (blen+j-BLArray::WORD_SIZE));
      array[i+1] = (array[i+1] & mask)| x >> (BLArray::WORD_SIZE-j);
   }
}
*/

bool BLArray::save(FILE * fp) {
    ulong wr = 0;
    wr += fwrite(&n, sizeof(uint), 1, fp);
    wr += fwrite(&blen, sizeof(uint), 1, fp);
    wr += fwrite(&arrayLength, sizeof(uint), 1, fp);
    wr += fwrite(&maxValue, sizeof(uint), 1, fp);
    wr += fwrite(array, sizeof(uint), arrayLength, fp);
    #ifdef VDEBUG
    cout << "Saving BitLength array" << endl;
    cout << "Expected: " << arrayLength+4 << endl;
    cout << "Got: " << wr << endl;
    #endif
    return wr == arrayLength+4;
}


bool BLArray::load(FILE * fp) {
    ulong wr = 0;
    wr += fread(&n, sizeof(uint), 1, fp);
    wr += fread(&blen, sizeof(uint), 1, fp);
    wr += fread(&arrayLength, sizeof(uint), 1, fp);
    wr += fread(&maxValue, sizeof(uint), 1, fp);
    array = new uint[arrayLength];
    if(array==NULL)
        return false;
    wr += fread(array, sizeof(uint), arrayLength, fp);
    #ifdef VDEBUG
    cout << "Loading BitLength array" << endl;
    cout << "Expected: " << arrayLength+4 << endl;
    cout << "Got: " << wr << endl;
    #endif
    return wr == (arrayLength+4);
}


uint BLArray::bits(uint maxValue) {
    uint b = 0;
    while(maxValue) {
        b++;
        maxValue >>= 1;
    }
    return b;
}


uint BLArray::getLength(uint n, uint blen) {
    return blen*n/BLArray::WORD_SIZE+1;
}


uint BLArray::size() {
    return (arrayLength+2)*sizeof(uint);
}
