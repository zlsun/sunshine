#ifndef INIT_H
#define INIT_H

#define CONCATENATE_DETAIL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_DETAIL(x, y)
#define MAKE_UNIQUE(x) CONCATENATE(x, __COUNTER__)

#define INIT_BEGIN_DETAIL(name)       \
    struct name {               \
        name() {

#define INIT_END_DETAIL(name)   \
        }                       \
    } name;

#define INIT_BEGIN INIT_BEGIN_DETAIL(MAKE_UNIQUE(_init_struct))

#define INIT_END INIT_END_DETAIL(MAKE_UNIQUE(_init_instance))

#endif // INIT_H
