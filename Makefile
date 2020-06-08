.PHONY: all test clean format tidy cpplint

FORMAT   = clang-format
TIDY     = clang-tidy
CPPLINT = cpplint

all:

test:
	cd test; $(MAKE) run-test

clean:
	rm -rf *~ include/*~
	cd test; $(MAKE) clean

format:
	$(FORMAT) --style=google -i ./simple_init_chain.h
	cd test; $(MAKE) format

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ ./simple_init_chain.h --  -std=c++11
	cd test; $(MAKE) tidy

cpplint:
	$(CPPLINT) ./simple_init_chain.h
	cd test; $(MAKE) cpplint



