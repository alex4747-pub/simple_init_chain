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
     $(COMMON)/CompA.cc \
     $(COMMON)/CompB.cc \
     $(COMMON)/CompC.cc \
     $(COMMON)/CompD.cc \
     $(COMMON)/CompE.cc \
     $(COMMON)/Recorder.cc

MAIN_SRCS = \
     test_main.cc

SRCS = $(COMMON_SRCS) $(MAIN_SRCS)

DEP_INCS = \
     $(COMMON)/Recorder.h \
     $(COMMON)/TestCommon.h \
     ../InitChain.h \
     ../InitChain.inl

INCS = \
     $(COMMON)/CompA.h \
     $(COMMON)/CompB.h \
     $(COMMON)/CompC.h \
     $(COMMON)/CompD.h \
     $(COMMON)/CompE.h \
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

