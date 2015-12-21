#ifndef ZMACRO_H
#define ZMACRO_H

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)

#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)

#endif // ZMACRO_H
