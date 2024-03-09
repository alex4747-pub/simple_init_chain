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

// This is the test component e
//
#include <comp_e.h>
#include <counter.h>
#include <simple_init_chain.h>
#include <tags.h>

#include <cassert>
#include <iostream>
#include <map>
#include <string>

// Initialization state
//
static bool comp_e_state;

bool GetCompEState() { return comp_e_state; }

// Initialization chain
//
// Concrete chain element
//
class InitWorkerE : public simple_init_chain::InitChain<CHAIN_TAG> {
 public:
  explicit InitWorkerE(int level) : InitChain<CHAIN_TAG>(level) {}

 private:
  bool InitFunc(std::map<std::string, std::string> const&) override {
    assert(!comp_e_state);
    std::cout << "Component-e: init function called: level=" << GetLevel()
              << std::endl;
    comp_e_state = true;
    CountInit(GetLevel());
    return true;
  }
};

// Static chain element
//
static InitWorkerE initWorkerE(30);
