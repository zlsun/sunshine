
MAKE = make --no-print-directory

.PHONY : test clean rebuild

test clean rebuild:
	@$(MAKE) -C ./tests $@
