
# project directories
I  =
S  = .
B  = build

# compile and link flags
INCLUDE_FLAGS = $(patsubst %,-I%,$(I))
CPPFLAGS      = $(INCLUDE_FLAGS)
CFLAGS        =
CXXFLAGS      = -Wall -std=c++1z -g
LDFLAGS       =
LDLIBS        = -lstdc++

# ===========================================================================

cpps = $(wildcard $S/*.cpp)
objs = $(patsubst $S/%,$B/%,$(cpps:.cpp=.o))
deps = $(objs:.o=.d)
exes = $(objs:.o=)

# ===========================================================================

.PHONY: test test_files

test: $(exes) test_files
	@$(foreach t,$(exes),./$t;)

test_files: $B/testdir/file

$B/testdir/file:
	@mkdir -p $(@D) && touch $@

$B/zpath: LDLIBS += -lboost_system -lboost_filesystem

# ===========================================================================

color = \e[$1m$2\e[0m
title = echo -e "$(call color,1;31,$1) $2"

$B/%: $B/%.o
	@$(call title,Linking,$^\t-> $@)
	@$(CC) $(LDFLAGS) $(LOADLIBES) $(LDLIBS) $^ -o $@

$B/%.o: $S/%.c
	@$(call title,Compiling,$<\t-> $@)
	@mkdir -p $(@D)
	@$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$B/%.o: $S/%.cpp
	@$(call title,Compiling,$<\t-> $@)
	@mkdir -p $(@D)
	@$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$B/%.d: $S/%.c
	@$(call title,Generating,$@)
	@mkdir -p $(@D)
	@$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< | sed 's,\($*\.o\),$B/\1 $@,g' > $@

$B/%.d: $S/%.cpp
	@$(call title,Generating,$@)
	@mkdir -p $(@D)
	@$(CC) -MM $(CPPFLAGS) $(CXXFLAGS) $< | sed 's,\($*\.o\),$B/\1 $@,g' > $@

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), rebuild)
-include $(deps)
endif
endif

.PHONY: clean rebuild

clean:
	@$(call title,Cleanning)
	@$(RM) -r $B

rebuild:
	@$(MAKE) clean
	@$(MAKE)
