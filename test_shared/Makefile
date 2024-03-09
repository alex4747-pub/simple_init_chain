STD=-std=c++11
COMMON = ../test_common

CXXFLAGS = -g -O0 -I.. -I../test_common -Wall -Wextra -Werror $(STD)

LDFLAGS = -L.

USE_GCC=yes

ifeq ($(USE_GCC),)
CXX = clang++
LIBS = -lc++ -ldl
else
CXX = g++
LIBS = -lstdc++ -ldl
endif

FORMAT  = clang-format
TIDY    = clang-tidy
CPPLINT = cpplint

LIB_SRCS = \
     $(COMMON)/comp_a.cc \
     $(COMMON)/comp_b.cc \
     $(COMMON)/comp_c.cc \
     $(COMMON)/comp_d.cc \
     $(COMMON)/comp_e.cc

COMMON_SRCS = \
     $(COMMON)/counter.cc

MAIN_SRCS = \
     test_main.cc

SRCS = $(LIB_SRCS) $(COMMON_SRCS) $(MAIN_SRCS)

LIB_INCS = \
     $(COMMON)/comp_a.h \
     $(COMMON)/comp_b.h \
     $(COMMON)/comp_c.h \
     $(COMMON)/comp_d.h \
     $(COMMON)/comp_e.h \
     $(COMMON)/tags.h

COMMON_INCS = \
     $(COMMON)/counter.h

INCS = $(LIB_INCS) $(COMMON_INCS)

TMP_LIB_SRCS = $(notdir $(LIB_SRCS))
LIB_OBJS = $(patsubst %.cc, %.o, $(TMP_LIB_SRCS))
SHLIBS = $(patsubst comp_%.cc, lib_comp_%.so, $(TMP_LIB_SRCS))

TMP_COMMON_SRCS = $(notdir $(COMMON_SRCS))
COMMON_OBJS = $(patsubst %.cc, %.o, $(TMP_COMMON_SRCS))

TMP_MAIN_SRCS = $(notdir $(MAIN_SRCS))
MAIN_OBJS = $(patsubst %.cc, %.o, $(TMP_MAIN_SRCS))

OBJS = $(MAIN_OBJS) $(COMMON_OBJS)


all: test_shared_init_chain
	echo $(SHLIBS)
	echo $(MAIN_OBJS)

test_shared_init_chain: $(OBJS) $(SHLIBS)
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(SHLIBS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(COMMON_INCS) ../simple_init_chain.h
	$(CXX) -c $(CXXFLAGS) -DCHAIN_TAG= -fpic $<

counter.o: $(COMMON)/counter.cc $(COMMON)/counter.h $(COMMON_INCS) ../simple_init_chain.h
	$(CXX) -c $(CXXFLAGS) -fpic $<

lib_comp_%.so: comp_%.o
	$(CXX) -o $@ -shared $(CXXFLAGS) $<

test_main.o: test_main.cc $(INCS) ../simple_init_chain.h
	$(CXX) -c $(CXXFLAGS) -fpic $<

format:
	$(FORMAT) --style=google -i $(SRCS) $(INCS)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) ../simple_init_chain.h -- $(CXXFLAGS) -DCHAIN_TAG=

cpplint:
	$(CPPLINT) $(SRCS) $(INCS) ../simple_init_chain.h

clean:
	rm -rf test_shared_init_chain *.o *.so *~ *.dSYM

run-test: test_shared_init_chain
	@echo
	@echo "Main test"
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./test_shared_init_chain
	@echo

