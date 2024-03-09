STD=-std=c++11

CXXFLAGS = -g -O0 -I.. -Wall -Wextra -Werror $(STD)

USE_GCC=yes

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

INCS=
SRCS = test_main.cc


all: test_delete_init_chain

test_delete_init_chain: test_main.cc ../simple_init_chain.h
	$(CXX) -o $@ $(CXXFLAGS) test_main.cc $(LIBS)

format:
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) ../simple_init_chain.h -- $(CXXFLAGS)

cpplint:
	$(CPPLINT) $(SRCS) $(INCS) ../simple_init_chain.h

clean:
	rm -rf test_delete_init_chain  *~ *.dSYM

run-test: test_delete_init_chain
	@echo
	@echo "Main test"
	./test_delete_init_chain
	@echo
