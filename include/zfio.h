#ifndef ZFIO_H
#define ZFIO_H

#include <iostream> // Don't delete this line!
#include <fstream>

namespace std {

ifstream fin(IN_FILE);
ofstream fout(OUT_FILE);

}

#define cin fin
#define cout fout

#include "zlog.h"

#endif // ZFIO_H
