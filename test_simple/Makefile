STD=-std=c++11
COMMON = ../test_common

CXXFLAGS = -g -O0 -I.. -I. -I$(COMMON) -Wall -Wextra -Werror $(STD)

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

COMMON_SRCS = \
     $(COMMON)/comp_a.cc \
     $(COMMON)/comp_b.cc \
     $(COMMON)/comp_c.cc \
     $(COMMON)/comp_d.cc \
     $(COMMON)/comp_e.cc \
     $(COMMON)/counter.cc

MAIN_SRCS = \
     test_main.cc

SRCS = $(COMMON_SRCS) $(MAIN_SRCS)

COMMON_INCS = \
     $(COMMON)/comp_a.h \
     $(COMMON)/comp_b.h \
     $(COMMON)/comp_c.h \
     $(COMMON)/comp_d.h \
     $(COMMON)/comp_e.h \
     $(COMMON)/counter.h \
     $(COMMON)/tags.h


INCS = $(COMMON_INCS)

TMP_COMMON_SRCS = $(notdir $(COMMON_SRCS))
COMMON_OBJS = $(patsubst %.cc, %.o, $(TMP_COMMON_SRCS))

all: test_simple_init_chain

test_simple_init_chain: test_main.o $(COMMON_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) test_main.o $(COMMON_OBJS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(COMMON)/counter.h $(COMMON)/tags.h ../simple_init_chain.h
	$(CXX) -c $(CXXFLAGS) -DCHAIN_TAG= $<

test_main.o: test_main.cc $(INCS) ../simple_init_chain.h
	$(CXX) -c $(CXXFLAGS) $<

format:
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) ../simple_init_chain.h -- $(CXXFLAGS) -DCHAIN_TAG=

cpplint:
	$(CPPLINT) $(SRCS) $(INCS) ../simple_init_chain.h

clean:
	rm -rf test_simple_init_chain *.o *~ *.dSYM

run-test: test_simple_init_chain
	@echo
	@echo "Main test"
	./test_simple_init_chain
	@echo
	@echo
	@echo "Exception test"
	./test_simple_init_chain -e
	@echo
	@echo
	@echo "Failure test"
	./test_simple_init_chain -f
	@echo
