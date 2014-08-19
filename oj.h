#ifndef OJ_H
#define OJ_H

#include <iostream> // Don't delete this line!
#include <fstream>

namespace std {

ifstream fin("in.txt");
ofstream fout("out.txt");

}

#define cin fin
#define cout fout

#include "zlog.h"

#endif // OJ_H
