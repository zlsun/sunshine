#ifndef ZTRACE_H
#define ZTRACE_H

#include <iostream>

#define trace(x) std::cout << #x << std::endl; x

#define D(x) std::cout << #x << " = " << x << std::endl

#endif // ZTRACE_H