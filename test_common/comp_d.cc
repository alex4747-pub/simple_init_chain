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

// This is the test component d
//

#include <comp_d.h>
#include <counter.h>
#include <simple_init_chain.h>
#include <tags.h>

#include <cassert>
#include <exception>
#include <iostream>
#include <map>
#include <string>

// Initialization state
//
static bool comp_d_state;
static bool comp_d_failure;
static bool comp_d_exception;

bool GetCompDState() { return comp_d_state; }

// Initialization chain
//

// Functions
//
void ArmDFailure() {
  assert(!comp_d_state);
  assert(!comp_d_failure);
  assert(!comp_d_exception);

  comp_d_failure = true;
}

void ArmDException() {
  assert(!comp_d_state);
  assert(!comp_d_failure);
  assert(!comp_d_exception);

  comp_d_exception = true;
}

// Initialization chain
//
// Concrete chain element
//
class InitWorkerD : public simple_init_chain::InitChain<CHAIN_TAG> {
 public:
  explicit InitWorkerD(int level) : InitChain<CHAIN_TAG>(level) {}

 private:
  bool InitFunc(std::map<std::string, std::string> const&) override {
    assert(!comp_d_state);

    if (comp_d_failure) {
      std::cout << "Component-d: init function called: level=" << GetLevel()
                << " will fail" << std::endl;
      comp_d_failure = false;
      return false;
    }

    if (comp_d_exception) {
      std::cout << "Component-d: init function called: level=" << GetLevel()
                << " will throw exception" << std::endl;
      comp_d_exception = false;
      throw std::logic_error("test exception");
    }

    std::cout << "Component-d: init function called: level=" << GetLevel()
              << std::endl;
    comp_d_state = true;
    CountInit(GetLevel());
    return true;
  }

  void ResetFunc(std::map<std::string, std::string> const&) override {
    std::cout << "Componnet-d: reset function called: level=" << GetLevel()
              << std::endl;
    comp_d_state = false;
    CountReset(GetLevel());
  }
};

// Static chain element
//
static InitWorkerD initWorkerD(25);
