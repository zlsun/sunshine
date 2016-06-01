
#include <zlog.h>

#include "zinit.h"
#include "zattribute.h"

NS_ZL_BEGIN

#ifdef ENABLE_ZLOG_TO
#ifndef WEAK_LINKED
std::ostream* Logger::os = &std::cout;
#endif
#endif

NS_ZL_END

