
MAKE = make --no-print-directory

targets = test clean rebuild

.PHONY : $(targets)
$(targets):
	@$(MAKE) -C ./tests $@

