STD=-std=c++11
CXXFLAGS = -g -O0 -I../include -I.

USE_GCC=

ifeq ($(USE_GCC),)
CXX = clang++
LIBS = -lc++
else
CXX = g++
LIBS = -lstdc++
endif

FORMAT  = clang-format
TIDY    = clang-tidy
CPPLINT = cpplint

SRCS = \
     test_main.cc \
     test_comp_a.cc \
     test_comp_b.cc

INCS = \
     test_comp_a.h \
     test_comp_b.h


all: test_simple_init_chain test_simple_init_chain_std

test_simple_init_chain: $(SRCS) $(INCS) ../include/simple_init_chain.h Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(SRCS) $(LIBS)

test_simple_init_chain_std: $(SRCS) $(INCS) ../include/simple_init_chain.h Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(STD) $(SRCS) $(LIBS)

format:
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) ../include/simple_init_chain.h -- $(CXXFLAGS) $(STD)

cpplint:
	$(CPPLINT) $(SRCS) $(INCS) ../include/simple_init_chain.h

clean:
	rm -rf test_simple_init_chain test_simple_init_chain_std *~ *.dSYM

run-test: test_simple_init_chain test_simple_init_chain_std
	@echo
	@echo "Test with all defaults"
	./test_simple_init_chain
	@echo
	@echo
	@echo "Std-test with all defaults"
	./test_simple_init_chain_std
	@echo

