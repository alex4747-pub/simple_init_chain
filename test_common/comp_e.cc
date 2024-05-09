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
#include <recorder.h>
#include <test_common.h>

#include <cassert>
#include <exception>
#include <iostream>
#include <string>

class CompE::Helper final : public INIT_CHAIN::Link {
 public:
  Helper(CompE* owner, int level)
      : Link(level, std::bind(&CompE::Helper::Init, this),
             std::bind(&CompE::Helper::Reset, this)),
        owner_(owner) {}

 private:
  bool Init() {
    std::cout << "Component-e: init function called: level=" << GetLevel()
              << std::endl;
    Recorder::SetState("e", true);
    Recorder::CountInit(GetLevel());

    owner_->init_done_ = true;

    if (GetLevel() & 0x1) {
      // Demonstrate deletion of link from inside
      // Init function
      owner_->helper_.reset();
    }

    return true;
  }

  bool Reset() {
    std::cout << "Component-e: reset function called: level=" << GetLevel()
              << std::endl;
    Recorder::SetState("e", false);
    Recorder::CountReset(GetLevel());

    owner_->init_done_ = false;

    if (GetLevel() & 0x2) {
      // Demonstrate deletion of link from inside
      // Reset function
      owner_->helper_.reset();
    }

    return true;
  }
  CompE* owner_;
};

CompE::CompE(int val)
    : val_(val), init_done_(), helper_(new Helper(this, 40 + val)) {}

bool CompE::Check() noexcept { return true; }

// Static instances of comp-e class
static CompE compE1(1);
static CompE compE2(2);
