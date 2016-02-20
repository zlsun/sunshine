#ifndef ZSTDRO_H
#define ZSTDRO_H

#include <cstdio>

#include "zcommon.h"
#include "zinit.h"

NS_ZL_BEGIN

#if defined(IN_FILE) && defined(OUT_FILE)

INIT
{
    std::freopen(IN_FILE, "r", stdin);
    std::freopen(OUT_FILE, "w", stdout);
}

#endif

NS_ZL_END

#endif // ZSTDRO_H
