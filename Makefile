.PHONY: all test clean format tidy cpplint

FORMAT   = clang-format
TIDY     = clang-tidy
CPPLINT = cpplint

all:

test:
	cd test_shared; $(MAKE) run-test
	cd test_simple; $(MAKE) run-test
	cd test_tagged; $(MAKE) run-test

clean:
	rm -rf *~ include/*~
	cd test_shared; $(MAKE) clean
	cd test_simple; $(MAKE) clean
	cd test_tagged; $(MAKE) clean

# It will format the test-common 3 times
# but it is acceptable
format:
	$(FORMAT) --style=google -i ./simple_init_chain.h
	cd test_shared; $(MAKE) format
	cd test_simple; $(MAKE) format
	cd test_tagged; $(MAKE) format



# It will tidy the test-common 3 times
# but it is acceptable
tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ ./simple_init_chain.h --  -std=c++17
	cd test_shared; $(MAKE) tidy
	cd test_simple; $(MAKE) tidy
	cd test_tagged; $(MAKE) tidy



# It will lin the test-common 3 times
# but it is acceptable
cpplint:
	$(CPPLINT) ./simple_init_chain.h
	cd test_shared; $(MAKE) cpplint
	cd test_simple; $(MAKE) cpplint
	cd test_tagged; $(MAKE) cpplint
