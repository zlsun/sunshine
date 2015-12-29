
# project directories
I  =
S  = .
B  = build

# compile and link flags
INCLUDE_FLAGS = $(patsubst %,-I%,$(I))
CPPFLAGS      = $(INCLUDE_FLAGS)
CXXFLAGS      = -Wall -std=c++1z -g
LDFLAGS       =
LDLIBS        = -lstdc++ -lboost_system -lboost_filesystem

color = \e[1;31m
reset = \e[0m
header = "$(color)$(1)$(reset) $(2)"

cpps = $(wildcard $S/*.cpp)
objs = $(patsubst $S/%, $B/%, $(cpps:.cpp=.o))
deps = $(objs:.o=.d)
tests = $(objs:.o=)

# phony targets
.PHONY : test test_filesystem clean rebuild

test: $(tests) test_filesystem
	@$(foreach t, $(tests), ./$t;)

test_filesystem: $B/testdir/file

$B/testdir/file:
	@mkdir -p $(@D)
	@touch $@

$B/%: $B/%.o
	@echo -e $(call header,Linking,$^\t-> $@)
	@$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o $@

$B/%.o: $S/%.cpp
	@echo -e $(call header,Compiling,$<\t-> $@)
	@mkdir -p $(@D)
	@$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$B/%.d: $S/%.cpp
	@echo -e $(call header,Generating,$@)
	@mkdir -p $(@D)
	@$(CC) -MM $(CPPFLAGS) $(CXXFLAGS) $< | sed 's,\($*\.o\),$B/\1 $@,g' > $@

# prevent make from generating dependencies when running 'make clean' or 'make rebuild'
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), rebuild)
-include $(deps)
endif
endif

clean:
	@echo -e $(call header,Cleanning)
	@$(RM) -r $B

rebuild:
	@$(MAKE) clean
	@$(MAKE)
