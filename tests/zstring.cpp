#include "../include/zstring.h"
#include "../include/ztest.h"
#include "../include/zlog.h"

using namespace std;
using namespace zl;

TEST(construct)
{
    String s = "1234";
    String ss(5, 's');
    const String cs {'1', '2', '3', '4'};
    EQUAL(s, "1234")
    EQUAL(ss, "sssss")
    EQUAL(cs, s)
}

TEST(access)
{
    String s = "1234";
    int l = s.length() - 1;
    EQUAL(l, 3)

    EQUAL(s[0], s.at(0))
    EQUAL(s[0], s.front())

    EQUAL(s[l], s.at(l))
    EQUAL(s[l], s.back())

    EQUAL(s[1], '2')
    s[1] = '0';
    EQUAL(s[1], '0')

    EQUAL(s.data(), s.c_str())
}

TEST(search)
{
    String s = "1234";
    EQUAL(s.find('1'), 0)
    EQUAL(s.find("4"), 3)
}

TEST(non_member)
{
    String s = "1234";
    String t = "2345";
    String r = "01234";
    const String cs = s;

    EQUAL(s + cs, "12341234")

    CHECK(s == cs)
    CHECK(s > r)
    CHECK(s < t)
}

TEST(extended)
{
    String ss(5, 's');
    EQUAL(ss.count('s'), 5)

    String abc = "aBc";
    EQUAL(abc.lower(), "abc")
    EQUAL(abc.upper(), "ABC")
    EQUAL(abc.capitalize(), "Abc")

    String trimed = " 1234 ";
    EQUAL(trimed.trim(), "1234");
    EQUAL(trimed.trim_left(), "1234 ");
    EQUAL(trimed.trim_right(), " 1234");

    String s = "1234";
    CHECK(s.startswith('1'))
    CHECK(s.endswith('4'))
    CHECK(s.contains("12"));

    String joined = " "_s.join({"123", "234", s});
    EQUAL(joined, "123 234 1234")
    EQUAL(joined.split(), (vector<String> {"123", "234", "1234"}));

    EQUAL(","_s.join({"1", "2"}).split(','), (vector<String> {"1", "2"}))

    EQUAL("1, 2, 3, 4"_s.replace_all(", ", "|"), "1|2|3|4")
}

TEST(slice)
{
    String s = "1234";

    EQUAL(s.slice(1), "234")
    EQUAL(s.slice(1, 2), "2")

    s.slice(0, 2) = "34";
    EQUAL(s, "3434")
}

TEST(format)
{
    auto format = "%d %d"_f;
    EQUAL(String(format % 1 % 2), "1 2")
}

TEST_MAIN
