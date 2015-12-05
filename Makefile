
.PHONY : test clean rebuild

test clean rebuild:
	@make -C ./tests  $@

