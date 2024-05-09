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

#include <comp_b.h>
#include <recorder.h>

#include <cassert>
#include <iostream>
#include <string>

CompB* CompB::self_;
int const CompB::init_level_ = 15;

CompB& CompB::GetInstance() noexcept {
  assert(self_ != nullptr);
  return *self_;
}

int CompB::GetCounter() noexcept {
  counter_++;
  return counter_;
}

bool CompB::Check() noexcept { return true; }

CompB::CompB() noexcept : counter_() {
  assert(self_ == nullptr);
  self_ = this;
}

CompB::~CompB() {
  assert(self_ == this);
  self_ = nullptr;
}

bool CompB::Init() {
  std::cout << "Component-b: init function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("b", true);
  Recorder::CountInit(init_level_);

  if (self_) {
    // Duplicate inits should not happen
    assert(false);
    return true;
  }

  CompB* app = new CompB;
  (void)app;

  // Allow reset operations
  return true;
}

bool CompB::Reset() {
  std::cout << "Component-b: reset function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("b", false);
  Recorder::CountReset(init_level_);

  // Demonstrate delete of the owning singleton
  // allow operations to continue
  delete self_;
  return true;
}

// Static chain element
//
INIT_CHAIN::Link CompB::init_helper_(init_level_, Init, Reset);
