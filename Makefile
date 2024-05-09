.PHONY: all test clean format tidy cpplint

FORMAT   = clang-format
TIDY     = clang-tidy
CPPLINT = cpplint

STD = -std=c++11

all:

run-test:
	cd test_namespace; $(MAKE) run-test
	cd test_shared; $(MAKE) run-test
	cd test_simple; $(MAKE) run-test
	cd test_tagged; $(MAKE) run-test

clean:
	rm -rf *~ include/*~
	cd test_namespace; $(MAKE) clean
	cd test_shared; $(MAKE) clean
	cd test_simple; $(MAKE) clean
	cd test_tagged; $(MAKE) clean


# It will format the test-common 4 times
# but it is acceptable
format:
	$(FORMAT) --style=google -i ./init_chain.h
	$(FORMAT) --style=google -i ./init_chain_tagged.h
	$(FORMAT) --style=google -i ./init_chain.inc
	cd test_namespace; $(MAKE) format
	cd test_shared; $(MAKE) format
	cd test_simple; $(MAKE) format
	cd test_tagged; $(MAKE) format

# It will tidy the test-common 4 times
# but it is acceptable
tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ ./init_chain.h -- $(STD) -DRUNNING_CPP_TIDY=1
	cd test_namespace; $(MAKE) tidy
	cd test_shared; $(MAKE) tidy
	cd test_simple; $(MAKE) tidy
	cd test_tagged; $(MAKE) tidy




# It will lint the test-common 4 times
# but it is acceptable
cpplint:
	$(CPPLINT) ./init_chain.h
	$(CPPLINT) ./init_chain_tagged.h
	$(CPPLINT) ./init_chain.inc
	cd test_namespace; $(MAKE) cpplint
	cd test_shared; $(MAKE) cpplint
	cd test_simple; $(MAKE) cpplint
	cd test_tagged; $(MAKE) cpplint
