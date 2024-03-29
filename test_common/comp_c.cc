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

// This is the test component c
//
#include <comp_c.h>
#include <counter.h>
#include <simple_init_chain.h>
#include <tags.h>

#include <cassert>
#include <iostream>
#include <map>
#include <string>

// Initialization state
//
static bool comp_c_state;

bool GetCompCState() { return comp_c_state; }

// Initialization chain
//
// Concrete chain element
//
class InitWorkerC : public simple_init_chain::InitChain<CHAIN_TAG> {
 public:
  explicit InitWorkerC(int level) : InitChain<CHAIN_TAG>(level) {}

 private:
  bool InitFunc(std::map<std::string, std::string> const&) override {
    assert(!comp_c_state);
    std::cout << "Component-c: init function called: level=" << GetLevel()
              << std::endl;
    comp_c_state = true;
    CountInit(GetLevel());
    return true;
  }
};

// Static chain element
//
static InitWorkerC initWorkerC(20);
