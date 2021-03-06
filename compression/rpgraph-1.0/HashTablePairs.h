/* HashTablePairs.h
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

#ifndef HASHTABLEPAIRS_H
#define HASHTABLEPAIRS_H

#include "basic.h"

#define maxAlpha 0.6

class HashTablePairs
{
    public:
        HashTablePairs(int * buff, uint buffLen);
        ~HashTablePairs();
        uint get(uint p1, uint p2);
        bool insert(uint p1, uint p2, uint value);
        uint h(uint p1, uint p2);
        uint s(uint p1, uint p2);

        uint buffLen;
        int *buff;
        long long queries;
        long long inserted;
        long long collisions;
};
#endif
