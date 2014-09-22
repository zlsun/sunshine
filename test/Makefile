
tests := zalgo zlog zrequire ztest
tests := $(addsuffix _test, $(tests))

CPPFLAGS += -std=c++11

all:
	@$(foreach t,$(tests),./$(t);)

.PHONY: all
