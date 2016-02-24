#include "zregex.h"
#include "zpipe.h"
#include "ztest.h"

using namespace zl;

TEST(basic)
{
    auto re = R"(\d)"_r;
    re = R"(\w)"_r;
    EQUAL(re.flags(), Regex::Flag::ECMAScript);
}

TEST(match)
{
    auto re = R"(([a-z]+)\.txt)"_r;
    String fnames[] = {"foo.txt", "bar.txt", "baz.dat", "zoidberg"};
    bool ok[] = {true, true, false, false};
    String bases[] = {"foo", "bar", "baz"};
    for (auto i : irange(4)) {
        EQUAL(re.is_match(fnames[i]), ok[i])
        if (re.is_match(fnames[i])) {
            EQUAL(re.match(fnames[i])[1].str(), bases[i])
        }
    }
}

TEST(search)
{
    String lines[] = {
        "Roses are #ff0000",
        "violets are #0000ff",
        "all of my base are belong to you"
    };
    auto re = "([a-f0-9]{2})([a-f0-9]{2})([a-f0-9]{2})"_r;
    bool ok[] = {true, true, false};
    String colors[] = {"ff0000", "0000ff"};
    for (auto i : irange(3)) {
        EQUAL(re.found(lines[i]), ok[i])
        if (re.found(lines[i])) {
            EQUAL(re.search(lines[i])[0].str(), colors[i])
        }
    }
}

TEST(replace)
{
    auto text = "Quick brown fox"_s;
    auto re = "a|e|i|o|u"_r;
    auto s = re.replace(text, "[$&]");
    EQUAL(s, "Q[u][i]ck br[o]wn f[o]x")
    s = re.replace("Qi", "[$&]");
    EQUAL(s, "Q[i]")
}

TEST_MAIN
