
cpps := $(wildcard *_test.cpp) ../src/ztest.cpp
objs := $(patsubst %.cpp, %.o, $(cpps))

CXXFLAGS += -std=c++11 -Wall

test_main: $(objs)
	gcc $^ -o main -lstdc++
	./main

*_test.o: ../include/*.h

clean:
	$(RM) $(objs) *.exe

.PHONY: clean
