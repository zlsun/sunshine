#ifndef ZMACRO_H
#define ZMACRO_H

#define CONCATENATE_IMPL(x, y) x ## y
#define CONCATENATE(x, y) CONCATENATE_IMPL(x, y)

#define UNIQUE_IDENTIFIER(x) CONCATENATE(x, __COUNTER__)

#define LAMBDA_RETURN(expr)   { return expr; }
#define LAMBDA_ARGUMENTS(...) (__VA_ARGS__) LAMBDA_RETURN
#define LAMBDA                [    ] LAMBDA_ARGUMENTS
#define CAPTURE(...)          [__VA_ARGS__] LAMBDA_ARGUMENTS

#define ARGS_SIZE_NUMBER_SEQ \
    32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \
    16, 15, 14, 13, 12, 11, 10,  9,  8,  7,  6,  5,  4,  3,  2,  1, 0
#define ARGS_SIZE_PREFIX_ARGS_SIZE_POSTFIX \
    ,,,, ,,,, ,,,, ,,,, ,,,, ,,,, ,,,, ,,,, 0
#define ARGS_SIZE_I( \
    p32, p31, p30, p29, p28, p27, p26, p25, p24, p23, p22, p21, p20, p19, p18, p17, \
    p16, p15, p14, p13, p12, p11, p10, p9,  p8,  p7,  p6,  p5,  p4,  p3,  p2,  p1,  n, ...) n
#define ARGS_SIZE_II(args) ARGS_SIZE_I args
#define ARGS_SIZE(...) ARGS_SIZE_II((ARGS_SIZE_PREFIX_ ## __VA_ARGS__ ## ARGS_SIZE_POSTFIX, ARGS_SIZE_NUMBER_SEQ))

#define ARGS_SIZE_V2(...) (LAMBDA(auto... x)(sizeof...(x))(__VA_ARGS__))

#endif // ZMACRO_H
