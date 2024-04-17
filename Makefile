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
	$(FORMAT) --style=google -i ./InitChain.h
	$(FORMAT) --style=google -i ./InitChainTagged.h
	$(FORMAT) --style=google -i ./InitChain.inl
	cd test_namespace; $(MAKE) format
	cd test_shared; $(MAKE) format
	cd test_simple; $(MAKE) format
	cd test_tagged; $(MAKE) format

# It will tidy the test-common 4 times
# but it is acceptable
tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ ./InitChain.h -- $(STD) -DRUNNING_CPP_TIDY=1
	cd test_namespace; $(MAKE) tidy
	cd test_shared; $(MAKE) tidy
	cd test_simple; $(MAKE) tidy
	cd test_tagged; $(MAKE) tidy




# It will lint the test-common 4 times
# but it is acceptable
cpplint:
	$(CPPLINT) ./InitChain.h
	$(CPPLINT) ./InitChainTagged.h
	$(CPPLINT) ./InitChain.inl
	cd test_namespace; $(MAKE) cpplint
	cd test_shared; $(MAKE) cpplint
	cd test_simple; $(MAKE) cpplint
	cd test_tagged; $(MAKE) cpplint
