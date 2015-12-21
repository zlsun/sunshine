#ifndef INIT_H
#define INIT_H

#include "zmacro.h"

#define INIT_IMPL(name, instance, function) \
    void function();                        \
    struct name                             \
    {                                       \
        name() {                            \
            function();                     \
        }                                   \
    } instance;                             \
    void function()

#define INIT INIT_IMPL(MAKE_UNIQUE(_init_struct),   \
                       MAKE_UNIQUE(_init_instance), \
                       MAKE_UNIQUE(_init_function))

#endif // INIT_H
