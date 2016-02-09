#ifndef INIT_H
#define INIT_H

#include "zmacro.h"

#define INIT_IMPL(name, instance, function) \
    static void function();                 \
    static struct name                      \
    {                                       \
        name() {                            \
            function();                     \
        }                                   \
    } instance;                             \
    void function()

#define INIT INIT_IMPL(UNIQUE_IDENTIFIER(_init_struct),   \
                       UNIQUE_IDENTIFIER(_init_instance), \
                       UNIQUE_IDENTIFIER(_init_function))

#endif // INIT_H
