STD=-std=c++17
CXXFLAGS = -g -O0 -I.. -I.

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

SRCS = \
	comp_a.cc \
	comp_b.cc \
	comp_c.cc \
	example.cc

all: example

example: $(SRCS) $(INCS) ../simple_init_chain.h Makefile
	$(CXX) -o $@ $(CXXFLAGS) $(STD) $(SRCS) $(LIBS)

format:
	$(FORMAT) --style=google -i $(SRCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) ../simple_init_chain.h -- $(CXXFLAGS) $(STD)

cpplint:
	$(CPPLINT) $(SRCS) ../simple_init_chain.h

clean:
	rm -rf example *~ *.dSYM

run: example
	./example

