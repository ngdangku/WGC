/* Heap.h
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   Heap

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

#ifndef HEAP_H
#define HEAP_H

#include "basic.h"
#include <cassert>
#include <utility>

using namespace std;

class Heap
{
    public:
        Heap(uint k);
        ~Heap();
        void insert(pair<uint,uint>* pos);

        pair<uint,uint> ** elements;
        uint inserted, k;
};
#endif
