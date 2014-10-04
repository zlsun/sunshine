
tests := $(wildcard *_test.cpp)
tests := $(patsubst %.cpp, %, $(tests))

CPPFLAGS += -std=c++11

all: $(tests)
	@$(foreach t,$(tests),./$(t);)

.PHONY: all
