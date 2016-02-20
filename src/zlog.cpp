
#include <zlog.h>

#include "zinit.h"

NS_ZL_BEGIN

#ifdef ENABLE_ZLOG_TO

std::ostream* Logger::os = &std::cout;

#endif

NS_ZL_END

