#ifndef ZSTDRO_H
#define ZSTDRO_H

#include <cstdio>

#include "zcommon.h"

NS_ZL_BEGIN

#if defined(IN_FILE) && defined(OUT_FILE)

struct __stdreopen
{
    __stdreopen()
    {
        std::freopen(IN_FILE, "r", stdin);
        std::freopen(OUT_FILE, "w", stdout);
    }
} __stdreopen_instance;

#endif

NS_ZL_END

#endif // ZSTDRO_H
