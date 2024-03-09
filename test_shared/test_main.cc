// Copyright (C) 2020  Aleksey Romanov (aleksey at voltanet dot io)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
#include <comp_a.h>
#include <comp_b.h>
#include <comp_c.h>
#include <getopt.h>
#include <simple_init_chain.h>
// Component d is not loaded automatically
#include <comp_e.h>
#include <counter.h>
#include <dlfcn.h>

#include <cassert>
#include <iostream>

static void usage() { std::cout << "usage: test_shared_init_chain\n"; }

int main(int argc, char**) {
  if (argc != 1) {
    std::cout << "unexpected parameters\n";
    usage();
    return 1;
  }

  assert(GetInitMap().size() == 0);
  assert(GetResetMap().size() == 0);

  // Run init chain
  bool res = simple_init_chain::InitChain<>::Run();
  assert(res);

  // Check state of linked libraries
  assert(GetCompAState());
  assert(GetCompBState());
  assert(GetCompCState());
  assert(GetCompEState());

  assert(GetInitMap().size() == 4);
  assert(GetResetMap().size() == 0);

  // Load component D using dlopen
  void* dla_handle = dlopen("lib_comp_d.so", RTLD_NOW);
  assert(dla_handle);

  // Run init chain again
  res = simple_init_chain::InitChain<>::Run();
  assert(res);

  // Make sure component D is loaded and
  // initialized
  assert(GetInitMap().size() == 5);
  assert(GetResetMap().size() == 0);

  return 0;
}
