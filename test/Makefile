
cpps := $(wildcard *_test.cpp) ../src/ztest.cpp
objs := $(patsubst %.cpp, %.o, $(cpps))

CXXFLAGS += -std=c++11

test_main: $(objs)
	gcc $^ -o main -lstdc++
	./main

clean:
	$(RM) $(objs) *.exe

.PHONY: clean
