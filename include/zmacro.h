#ifndef ZMACRO_H
#define ZMACRO_H

#define CONCATENATE_IMPL(x, y) x##y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)

#define UNIQUE_IDENTIFIER(x) CONCATENATE(x, __COUNTER__)

#define ARGS_SIZE_PREFIX
#define ARGS_SIZE_HELPER(_, p10, p9, p8, p7, p6, p5, p4, p3, p2, p1, n, ...) n
#define ARGS_SIZE(...) ARGS_SIZE_HELPER(ARGS_SIZE_PREFIX,##__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#endif // ZMACRO_H
