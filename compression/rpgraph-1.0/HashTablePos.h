/* HashTablePos.h
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

#ifndef HASHTABLEPOS_H
#define HASHTABLEPOS_H

#include "basic.h"

#define maxAlpha 0.6
#define VERBOSE

class HashTablePos
{
    public:
        HashTablePos(uint * data, uint dataLen, int * buff, uint buffLen);
        ~HashTablePos();
        bool increment(uint pos);
        uint get(uint pos);
        bool insert(uint pos, uint value);
        uint h(uint pos);
        uint s(uint pos);
        bool cmp(uint pos1, uint pos2);

        uint dataLen, buffLen;
        uint *data;
        int *buff;
        long long queries;
        long long inserted;
        long long collisions;
};
#endif
