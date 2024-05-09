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
     $(COMMON)/even_tag.h \
     $(COMMON)/odd_tag.h \
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

all: test_namesp_init_chain

test_namesp_init_chain: test_main.o $(COMMON_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) test_main.o $(COMMON_OBJS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) $<

comp_a.o: $(COMMON)/comp_a.cc $(COMMON)/comp_a.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_NS_EVEN $<

comp_b.o: $(COMMON)/comp_b.cc $(COMMON)/comp_b.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_NS_ODD $<

comp_c.o: $(COMMON)/comp_c.cc $(COMMON)/comp_c.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_NS_EVEN $<

comp_d.o: $(COMMON)/comp_d.cc $(COMMON)/comp_d.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_NS_ODD $<

comp_e.o: $(COMMON)/comp_e.cc $(COMMON)/comp_e.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_NS_EVEN $<

test_main.o: test_main.cc $(INCS)
	$(CXX) -c $(CXXFLAGS) $<

format:
	$(FORMAT) --style=google -i $(SRCS) test_main.cc $(wildcard $(COMMON)/*.h)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(SRCS) $(INCS) test_main.cc -- $(CXXFLAGS) -DRUNNING_CPP_TIDY=1

cpplint:
	$(CPPLINT) $(SRCS) $(wildcard $(COMMON)/*.h)

clean:
	rm -rf test_namesp_init_chain *.o *~ *.dSYM $(COMMON)/*~

run-test: test_namesp_init_chain
	@echo
	@echo "Main test"
	./test_namesp_init_chain
	@echo

print:
	$(CXX) -E -c $(CXXFLAGS) test_main.cc



