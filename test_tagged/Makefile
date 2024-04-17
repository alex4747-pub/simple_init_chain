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
     $(COMMON)/EvenTag.h \
     $(COMMON)/OddTag.h \
     $(COMMON)/Recorder.h \
     $(COMMON)/TestCommon.h \
     ../InitChainTagged.h \
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

TIDY_SRCS_EVEN = \
     $(COMMON)/CompA.cc \
     $(COMMON)/CompC.cc \
     $(COMMON)/CompE.cc \
     $(COMMON)/Recorder.cc \
     ./test_main.cc	

TIDY_INCS_EVEN = \
     $(COMMON)/CompA.h \
     $(COMMON)/CompC.h \
     $(COMMON)/CompE.h \
     $(COMMON)/EvenTag.h \
     $(COMMON)/Recorder.h \
     $(COMMON)/TestCommon.h \
     ../InitChainTagged.h \

TIDY_SRCS_ODD = \
     $(COMMON)/CompB.cc \
     $(COMMON)/CompD.cc \
     $(COMMON)/Recorder.cc \
     ./test_main.cc	

TIDY_INCS_ODD = \
     $(COMMON)/CompB.h \
     $(COMMON)/CompD.h \
     $(COMMON)/OddTag.h \
     $(COMMON)/Recorder.h \
     $(COMMON)/TestCommon.h \
     ../InitChainTagged.h \

all: test_tagged_init_chain

test_tagged_init_chain: test_main.o $(COMMON_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) test_main.o $(COMMON_OBJS) $(LIBS)

%.o: $(COMMON)/%.cc $(COMMON)/%.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) $<

CompA.o: $(COMMON)/CompA.cc $(COMMON)/CompA.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_EVEN $<

CompB.o: $(COMMON)/CompB.cc $(COMMON)/CompB.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_ODD $<

CompC.o: $(COMMON)/CompC.cc $(COMMON)/CompC.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_EVEN $<

CompD.o: $(COMMON)/CompD.cc $(COMMON)/CompD.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_ODD $<

CompE.o: $(COMMON)/CompE.cc $(COMMON)/CompE.h $(DEP_INCS)
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_EVEN $<

test_main.o: test_main.cc $(INCS)
	$(CXX) -c $(CXXFLAGS) $<

format:
	$(FORMAT) --style=google -i $(SRCS) test_main.cc $(wildcard $(COMMON)/*.h)

tidy:
	$(TIDY) --fix -extra-arg-before=-xc++ $(TIDY_SRCS_EVEN) $(TIDY_SRCS_EVEN) -- $(CXXFLAGS) -DUSE_TAG_EVEN -DRUNNING_CPP_TIDY=2
	$(TIDY) --fix -extra-arg-before=-xc++ $(TIDY_SRCS_ODD) $(TIDY_SRCS_ODD) -- $(CXXFLAGS) -DUSE_TAG_ODD -DRUNNING_CPP_TIDY=2

cpplint:
	$(CPPLINT) $(SRCS) $(wildcard $(COMMON)/*.h)

clean:
	rm -rf test_tagged_init_chain *.o *~ *.dSYM $(COMMON)/*~

run-test: test_tagged_init_chain
	@echo
	@echo "Main test"
	./test_tagged_init_chain
	@echo

print:
	$(CXX) -c $(CXXFLAGS) -DUSE_TAG_EVEN -DRUNNING_CPP_TIDY=2 ../test_common/CompA.cc


