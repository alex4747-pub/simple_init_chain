STD=-std=c++11
CXXFLAGS = -g -O0 -I.. -I. -Wall -Wextra -Werror

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
     test_comp_b.cc \
     test_comp_c.cc \
     test_comp_d.cc \
     test_comp_e.cc \
     test_counter.cc

INCS = \
     test_comp_a.h \
     test_comp_b.h \
     test_comp_c.h \
     test_comp_d.h \
     test_comp_e.h \
     test_counter.h


all: test_simple_init_chain test_simple_init_chain_std

test_simple_init_chain: $(SRCS) $(INCS) ../simple_init_chain.h Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(SRCS) $(LIBS)

test_simple_init_chain_std: $(SRCS) $(INCS) ../simple_init_chain.h Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(STD) $(SRCS) $(LIBS)

format:
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) ../simple_init_chain.h -- $(CXXFLAGS) $(STD)

cpplint:
	$(CPPLINT) $(SRCS) $(INCS) ../simple_init_chain.h

clean:
	rm -rf test_simple_init_chain test_simple_init_chain_std *~ *.dSYM

run-test: test_simple_init_chain test_simple_init_chain_std
	@echo
	@echo "Main test"
	./test_simple_init_chain
	@echo
	@echo
	@echo "Main std-test"
	./test_simple_init_chain_std
	@echo
	@echo
	@echo "Exception test"
	./test_simple_init_chain -e
	@echo
	@echo
	@echo "Exception std-test"
	./test_simple_init_chain_std -e
	@echo
	@echo
	@echo "Failure test"
	./test_simple_init_chain -f
	@echo
	@echo
	@echo "Failure std-test"
	./test_simple_init_chain_std -f

