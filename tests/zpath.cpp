#include "../include/zpath.h"
#include "../include/zinit.h"
#include "../include/zlog.h"
#include "../include/ztest.h"
using namespace zl;

Path current_file;
Path current_dir;
Path test_dir;
Path test_file;
Path test_file2;

INIT_BEGIN
    current_file = Path(__FILE__).absolute();
    current_dir = current_file.parent_path();
    test_dir = current_dir / "build" / "testdir";
    test_file = test_dir / "file";
    test_file2 = test_dir / "file2";
INIT_END

TEST(zpath)
    auto p = "/"_path;
    EQUAL(p / "home" / "zlsun", "/home/zlsun")
    p = "/"_s;
END

TEST(absolute)
    EQUAL("/"_path.absolute(), "/")
    EQUAL("."_path.absolute().parent_path(), current_dir)
END

TEST(canonical)
    EQUAL("."_path.canonical(), current_dir)
END

TEST(exists)
    EQUAL(test_file.exists(), true)
    EQUAL(test_file2.exists(), false)
END

// TEST(copy)
//     test_file.copy(test_file2);
//     EQUAL(test_file2.exists(), true)
// END

TEST(create_directories)
    Path test = test_dir / "test";
    test.create_directories();
    EQUAL(test.exists(), true)
END

TEST_MAIN
