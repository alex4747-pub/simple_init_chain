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

// This is the test component c.
//

#include <comp_c.h>
#include <recorder.h>
#include <test_common.h>

#include <cassert>
#include <iostream>
#include <string>

CompC* CompC::self_;
int const CompC::init_level_ = 20;

CompC& CompC::GetInstance() noexcept {
  assert(self_ != nullptr);
  return *self_;
}

int CompC::GetCounter() noexcept {
  counter_++;
  return counter_;
}

bool CompC::Check() noexcept { return true; }

CompC::CompC() noexcept : counter_() {
  assert(self_ == nullptr);
  self_ = this;
}

CompC::~CompC() {
  assert(self_ == this);
  self_ = nullptr;
}

class CompC::InitHelper : public INIT_CHAIN::Link {
 public:
  InitHelper();

  bool Init();
  bool Reset();
};

CompC::InitHelper::InitHelper()
    : Link(init_level_, std::bind(&CompC::InitHelper::Init, this),
           std::bind(&CompC::InitHelper::Reset, this)) {}

bool CompC::InitHelper::Init() {
  std::cout << "Component-c: init function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("c", true);
  Recorder::CountInit(init_level_);

  if (self_) {
    // Duplicate inits should not happen
    assert(false);

    // Resets (and repeated inits) are disabled by #define
#ifdef USE_RESET
    return true;
#else
    return false;
#endif
  }

  CompC* app = new CompC;
  (void)app;

  // Resets (and repeated inits) are disabled by #define
#ifdef USE_RESET
  return true;
#else
  return false;
#endif
}

bool CompC::InitHelper::Reset() {
  std::cout << "Component-c: reset function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("c", false);
  Recorder::CountReset(init_level_);

  delete self_;
  return true;
}

// Static chain init helper
//
CompC::InitHelper CompC::init_helper_;
