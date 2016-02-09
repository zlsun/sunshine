#include "../include/ztest.h"
#include "../include/ztype.h"

using namespace zl;

using T = Typelist<int, char>; // <int, char>
using U = Append<double, T>;   // <int, char, double>
using V = Concat<T, U>;        // <int, char, int, char, double>
using W = Insert<0, float, V>; // <float, int, char, int, char, double>
using X = Pop<W>;              // <float, int, char, int, char>
using Y = Delete<0, X>;        // <int, char, int, char>

TEST(length)
{
    EQUAL(Length<T>, 2)
    EQUAL(Length<U>, 3)
    EQUAL(Length<V>, 5)
    EQUAL(Length<W>, 6)
    EQUAL(Length<X>, 5)
    EQUAL(Length<Y>, 4)
}

TEST(indexOf)
{
    EQUAL((IndexOf<bool, T>), -1)
    EQUAL((IndexOf<double, U>), 2)
    EQUAL((IndexOf<double, V>), 4)
    EQUAL((IndexOf<float, W>), 0)
}

TEST(toString)
{
    EQUAL(ToString<T>, "<int, char>")
    EQUAL(ToString<U>, "<int, char, double>")
    EQUAL(ToString<V>, "<int, char, int, char, double>")
    EQUAL(ToString<W>, "<float, int, char, int, char, double>")
    EQUAL(ToString<X>, "<float, int, char, int, char>")
    EQUAL(ToString<Y>, "<int, char, int, char>")
}

TEST_MAIN
