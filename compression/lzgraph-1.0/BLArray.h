/* BLArray.h
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

#ifndef BLARRAY_H
#define BLARRAY_H

#include <iostream>
#include <cassert>
#include "basic.h"

class BLArray
{
    public:
        BLArray(FILE * fp);
        BLArray(uint n, uint maxValue);
        ~BLArray();

        inline ulong getField(uint index) {
            register ulong i=(ulong)index*blen/BLArray::WORD_SIZE, j=(ulong)index*blen-BLArray::WORD_SIZE*i, result;
            //register ulong i=(index/W)*len+((index%W)*len)/W, j=((index%W)*len)%W,result;
            if (j+blen <= BLArray::WORD_SIZE)
                result = (array[i] << (BLArray::WORD_SIZE-j-blen)) >> (BLArray::WORD_SIZE-blen);
            else {
                result = array[i] >> j;
                result = result | (array[i+1] << (BLArray::DWORD_SIZE-j-blen)) >> (BLArray::WORD_SIZE-blen);
            }
            return result;
        }

        inline void setField(uint index, uint x) {
            ulong i=(ulong)index*blen/BLArray::WORD_SIZE, j=(ulong)index*blen-i*BLArray::WORD_SIZE;
            //register ulong i=(index/W)*len+((index%W)*len)/W, j=((index%W)*len)%W;
            ulong mask = ((j+blen) < BLArray::WORD_SIZE ? ~0uL << (j+blen) : 0) | ((BLArray::WORD_SIZE-j) < BLArray::WORD_SIZE ? ~0uL >> (BLArray::WORD_SIZE-j) : 0);
            array[i] = (array[i] & mask) | x << j;
            if (j+blen>BLArray::WORD_SIZE) {
                mask = ((~0uL) << (blen+j-BLArray::WORD_SIZE));
                array[i+1] = (array[i+1] & mask)| x >> (BLArray::WORD_SIZE-j);
            }
        }

        bool save(FILE * fp);
        bool load(FILE * fp);

        uint size();

    protected:
        uint * array;
        uint arrayLength;
        uint n, blen;
        uint maxValue;

        static const uint WORD_SIZE=8*sizeof(uint);
        static const uint DWORD_SIZE=2*WORD_SIZE;
        static uint bits(uint maxValue);
        static uint getLength(uint n, uint blen);
};
#endif
