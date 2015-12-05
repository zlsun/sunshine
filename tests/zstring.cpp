#include "../include/zstring.h"
#include "../include/ztest.h"
#include "../include/zlog.h"
using namespace std;
using namespace zl;

TEST(construct)
    String s = "1234";
    String ss(5, 's');
    const String cs = {'1', '2', '3', '4'};
END

TEST(access)
    String s = "1234";
    const String cs = s;
    EQUAL(s[0], '1')
    EQUAL(cs[1], '2')
END

TEST(search)
    String s = "1234";
    EQUAL(s.find('1'), 0)
    EQUAL(s.find('4'), 3)
END

TEST(non_member)
    String s = "1234";
    String t = "2345";
    String r = "01234";
    const String cs = s;

    EQUAL(s + cs, "12341234")

    TRUE(s == cs)
    TRUE(s > r)
    TRUE(s < t)
END

TEST(extended)
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
    TRUE(s.startswith('1'))
    TRUE(s.endswith('4'))
    TRUE(s.contains("12"));

    String joined = " "_s.join({"123", "234", s});
    EQUAL(joined, "123 234 1234")
    EQUAL(joined.split(), (vector<String> {"123", "234", "1234"}));
END

TEST(format)
    auto format = "%d %d"_f;
    EQUAL(String(format % 1 % 2), "1 2")
END

TEST_MAIN
