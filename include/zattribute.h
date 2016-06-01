#ifndef ZATTRIBUTE_H
#define ZATTRIBUTE_H

#if defined(__GNUC__) || defined(__clang__)
# define WEAK_LINKED __attribute__((weak))
#elif defined(_MSC_VER)
# define WEAK_LINKED __declspc(selectany)
#endif

#endif // ZATTRIBUTE_H
