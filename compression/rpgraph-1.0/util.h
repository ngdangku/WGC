/* util.h
   Copyright (C) 2007, Francisco Claude, all rights reserved.

   util

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

#ifndef UTIL_H
#define UTIL_H

#include <iostream>

using namespace std;

inline void my_fread(void * data, size_t sz, size_t num, FILE * fp) {
    size_t rd = fread(data,sz,num,fp);
    if(rd!=num) {
        cerr << "sz=" << sz << endl;
        cerr << "num=" << num << endl;
        cerr << "rd=" << rd << endl;
        cerr << "Error reading file" << endl;
        abort();
    }
}


inline void my_fwrite(void * data, size_t sz, size_t num, FILE * fp) {
    size_t wr = fwrite(data,sz,num,fp);
    if(wr!=num) {
        cerr << "Error writing file" << endl;
        abort();
    }
}
#endif
