/* LZGraph.h
   Copyright (C) 2007, Francisco Claude F, all rights reserved.

   LZGraph -- using differences

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

#ifndef lzgraph_h
#define lzgraph_h

#include "BLArray.h"
#include "bitrankw32int.h"
#include <iostream>

using namespace std;

class LZGraph
{
    public:
        uint n,e,ml;

        LZGraph(char * fname);
        ~LZGraph();

        uint * adj(uint n);
        uint outdegree(uint n);

        uint size();

    private:
        BLArray *s,*p,*ptr;
        uint *info;
};
#endif
