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

#ifndef REPAIRDECOMPRESSOR
#define REPAIRDECOMPRESSOR

#include <iostream>
#include <cassert>
#include "bitrankw32int.h"
#include "basic.h"

using namespace std;

class RepairGraph
{
    public:
        uint m;                  // Tamano final
        uint n;                  // Tamano original
        BitRankW32Int * BRR;     // Se construye pal arbolito
        uint *symbols;           // simbolos con reemplazos
        uint nbits;
        uint nodes, edges;
        uint e;
        uint * info;
        uint pos;
        BitRankW32Int * bptrs, *bnoz;
        uint * csymb_n, bits_sn;
        uint symbols_new_len;
        uint ml, max_value;

        RepairGraph(char * file);
        ~RepairGraph();
        uint size();
        void expand(uint v);
        uint * adj(int v);
        uint outdegree(int v);
};
#endif
