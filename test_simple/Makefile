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
     $(COMMON)/recorder.cc

MAIN_SRCS = \
     test_main.cc

SRCS = $(COMMON_SRCS) $(MAIN_SRCS)

DEP_INCS = \
     $(COMMON)/recorder.h \
     $(COMMON)/test_common.h \
     ../init_chain.h \
     ../init_chain.inc

INCS = \
     $(COMMON)/comp_a.h \
     $(COMMON)/comp_b.h \
     $(COMMON)/comp_c.h \
     $(COMMON)/comp_d.h \
     $(COMMON)/comp_e.h \
     $(COMP_INCS) \
     $(DEP_INCS)

TMP_COMMON_SRCS = $(notdir $(COMMON_SRCS))
COMMON_OBJS = $(patsubst %.cc, %.o, $(TMP_COMMON_SRCS))

all: test_simple_init_chain

test_simple_init_chain: test_main.o $(COMMON_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) test_main.o $(COMMON_OBJS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) $<

test_main.o: test_main.cc $(INCS)
	$(CXX) -c $(CXXFLAGS) $<

format:
	$(FORMAT) --style=google -i $(SRCS) test_main.cc $(wildcard $(COMMON)/*.h)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) test_main.cc $(INCS) -- $(CXXFLAGS) -DRUNNING_CPP_TIDY=1

cpplint:
	$(CPPLINT) $(SRCS) $(wildcard $(COMMON)/*.h)

clean:
	rm -rf test_simple_init_chain *.o *~ *.dSYM $(COMMON)/*~

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
	@echo
	@echo "Link release test"
	./test_simple_init_chain -l
	@echo
	@echo
	@echo "Release test"
	./test_simple_init_chain -r
	@echo

