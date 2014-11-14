#ifndef ZSTDRO_H
#define ZSTDRO_H

#include <cstdio>

struct __stdreopen {
    __stdreopen() {
        std::freopen(IN_FILE, "r", stdin);
        std::freopen(OUT_FILE, "w", stdout);
    }
} __stdreopen_instance;


#endif // ZSTDRO_H
