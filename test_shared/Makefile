STD=-std=c++11
COMMON = ../test_common

CXXFLAGS = -g -O0 -I.. -I. -I$(COMMON) -Wall -Wextra -Werror $(STD)

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
     $(COMMON)/recorder.cc

MAIN_SRCS = \
     test_main.cc

SRCS = $(LIB_SRCS) $(COMMON_SRCS) $(MAIN_SRCS)

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
     $(DEP_INCS)

TMP_LIB_SRCS = $(notdir $(LIB_SRCS))
LIB_OBJS = $(patsubst %.cc, %.o, $(TMP_LIB_SRCS))
SHLIBS = $(patsubst comp_%.cc, lib_comp_%.so, $(TMP_LIB_SRCS))

TMP_COMMON_SRCS = $(notdir $(COMMON_SRCS))
COMMON_OBJS = $(patsubst %.cc, %.o, $(TMP_COMMON_SRCS))

TMP_MAIN_SRCS = $(notdir $(MAIN_SRCS))
MAIN_OBJS = $(patsubst %.cc, %.o, $(TMP_MAIN_SRCS))

OBJS = $(MAIN_OBJS) $(COMMON_OBJS)

all: test_shared_init_chain

// Note: lib_comp_D will be loaded by dlopen
test_shared_init_chain: $(OBJS) $(SHLIBS)
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(SHLIBS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -fpic $<

test_main.o: test_main.cc $(INCS)
	$(CXX) -c $(CXXFLAGS) -fpic $<

lib_comp_%.so: comp_%.o
	$(CXX) -o $@ -shared $(CXXFLAGS) $<

format:
	$(FORMAT) --style=google -i $(SRCS) test_main.cc $(filter-out, ../ChainList.h $(INCS))

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) test_main.cc $(INCS) -- $(CXXFLAGS) -DRUNNING_CPP_TIDY=1

cpplint:
	$(CPPLINT) $(SRCS) $(INCS)

clean:
	rm -rf test_shared_init_chain *.o *~ *.dSYM $(COMMON)/*~ *.so

run-test: test_shared_init_chain
	@echo
	@echo "Main test"
	LD_LIBRARY_PATH=.:$$LD_LIBRARY_PATH ./test_shared_init_chain
	@echo

print:
	echo $(LIB_OBJS)
	echo $(TMP_LIB_SRCS)
	echo $(SHLIBS)


