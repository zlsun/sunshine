#ifndef ZREOPEN_H
#define ZREOPEN_H

#include <cstdio>

#include "zcommon.h"
#include "zinit.h"

NS_ZL_BEGIN

#if defined(IN_FILE) || defined(OUT_FILE)

INIT
{
#ifdef IN_FILE
    std::freopen(IN_FILE, "r", stdin);
#endif
#ifdef OUT_FILE
    std::freopen(OUT_FILE, "w", stdout);
#endif
}

#endif

NS_ZL_END

#endif // ZREOPEN_H
