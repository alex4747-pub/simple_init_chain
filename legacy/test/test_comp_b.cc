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

// This is the test component b
//

#include <simple_init_chain.h>
#include <test_comp_b.h>
#include <test_counter.h>
#include <cassert>
#include <iostream>

// Initialization state
//
static bool comp_b_state;

bool GetCompBState() { return comp_b_state; }

// Initialization chain
//

// Functions
//

static bool InitFunc(int level, simple::InitChain::ConfigMap const&) {
  assert(!comp_b_state);
  std::cout << "Component-b: init function called: level=" << level
            << std::endl;
  comp_b_state = true;
  CountInit(level);
  return true;
}

static void ResetFunc(int level, simple::InitChain::ConfigMap const&) {
  std::cout << "Componnet-b: reset function called: level=" << level
            << std::endl;
  comp_b_state = false;
  CountReset(level);
}

// Static chain element
//
static simple::InitChain::El compBInitChainEl(15, InitFunc, ResetFunc);
