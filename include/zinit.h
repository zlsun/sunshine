#ifndef INIT_H
#define INIT_H

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)

#define INIT_BEGIN_IMPL(name) \
    struct name {             \
        name() {

#define INIT_END_IMPL(name) \
        }                   \
    } name;

#define INIT_BEGIN INIT_BEGIN_IMPL(MAKE_UNIQUE(_init_struct))

#define INIT_END INIT_END_IMPL(MAKE_UNIQUE(_init_instance))

#endif // INIT_H
