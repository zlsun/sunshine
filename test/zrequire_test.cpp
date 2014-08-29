#include "../ztest.h"
#include "../zrequire.h"
using namespace std;

int exit_count = 0;
void exit(int status) {
    ++exit_count;
}

test_begin(zrequire);
    
    require(true);
    test_equal(exit_count, 0);
    require(false);
    test_equal(exit_count, 1);
    
    require(1 + 1 == 2);
    test_equal(exit_count, 1);
    require(1 + 1 == 3, "1 + 1 != 3");
    test_equal(exit_count, 2);

    ifstream fin("zrequire_test.cpp");
    assure(fin, "zrequire_test.cpp");
    test_equal(exit_count, 2);
    
    ofstream fout("");
    assure(fout, "");
    test_equal(exit_count, 3);

test_end();

test_main();
