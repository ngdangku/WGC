/* LempelZiv.h
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

#ifndef LempelZiv_h
#define LempelZiv_h

#include "basic.h"
#include "BLArray.h"
#include <iostream>
#include <stack>
#include <list>
#include <map>

using namespace std;

class LZTrie
{
    public:
        LZTrie(uint id);
        ~LZTrie();
        void add(uint id, uint symbol);
        LZTrie * get(uint symbol);
        uint getId();

    protected:
        uint id;
        map<uint, LZTrie* > c;
};

class LZPhrase
{
    public:
        LZPhrase(uint symbol, uint parent);
        ~LZPhrase();

        uint symbol, parent;
};

int compress(uint * data, uint n, BLArray ** symb, BLArray ** parent, uint ** br, uint * start);
uint * decompress(BLArray * symb, BLArray * parent, uint phrases, uint n);
#endif
