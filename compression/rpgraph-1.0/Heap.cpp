/* heap.cpp
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   heap

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

#include "Heap.h"

Heap::Heap(uint k) {
    elements = new pair<uint,uint>*[k+1];
    for(uint i=0;i<k+1;i++)
        elements[i]=NULL;
    assert(elements!=NULL);
    inserted = 1; this->k=k;
}


Heap::~Heap() {
    for(uint i=0;i<k+1;i++)
        if(elements[i]!=NULL)
            delete elements[i];
    delete [] elements;
}


void Heap::insert(pair<uint,uint> * elem) {
    if(inserted<=k) {            // Insercion normal
        elements[inserted] = elem;
        for(uint j=inserted;j>1 && elements[j]->second<elements[j/2]->second;j/=2) {
            pair<uint,uint> *t = elements[j/2];
            elements[j/2] = elements[j];
            elements[j] = t;
        }
        inserted++;
    }
                                 // Debemos eliminar alguno
    else if(elements[1]->second<elem->second) {
        delete elements[1];
        elements[1] = elem;
        uint j = 1;
        while(2*j<=k) {
            uint p = 2*j;
            if(p+1<=k && elements[p+1]->second<elements[p]->second)
                p++;
            if(elements[j]->second<elements[p]->second)
                break;
            pair<uint,uint> *t = elements[j];
            elements[j] = elements[p];
            elements[p] = t;
            j = p;
        }
    }
    else
        delete elem;
}
