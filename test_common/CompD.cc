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

#include <CompD.h>
#include <Recorder.h>
#include <TestCommon.h>

#include <cassert>
#include <exception>
#include <iostream>
#include <string>

CompD* CompD::self_;
bool CompD::failure_armed_;
bool CompD::exception_armed_;

int const CompD::init_level_ = 25;

CompD& CompD::GetInstance() noexcept {
  assert(self_ != nullptr);
  return *self_;
}

int CompD::GetCounter() noexcept {
  counter_++;
  return counter_;
}

void CompD::ArmFailure() noexcept { failure_armed_ = true; }

void CompD::ArmException() noexcept { exception_armed_ = true; }

CompD::CompD() noexcept : counter_() {
  assert(self_ == nullptr);
  self_ = this;
}

CompD::~CompD() {
  assert(self_ == this);
  self_ = nullptr;
}

// Initialization helper class
class CompD::Helper final : public INIT_CHAIN::Link {
 public:
  Helper();

 private:
  static bool Init();
  static bool Reset();
};

CompD::Helper::Helper() : Link(init_level_, Init, Reset) {}

bool CompD::Helper::Init() {
  if (failure_armed_) {
    std::cout << "Component-d: init function called: level=" << init_level_
              << " will fail" << std::endl;
    failure_armed_ = false;
    // Still return true to allow re-run
    // in test environment
    return true;
  }

  if (exception_armed_) {
    std::cout << "Component-d: init function called: level=" << init_level_
              << " will throw exception" << std::endl;
    exception_armed_ = false;
    throw std::logic_error("test exception");
  }

  std::cout << "Component-d: init function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("d", true);
  Recorder::CountInit(init_level_);

  if (self_) {
    // Douplicate init should not happen
    //
    assert(false);
    return true;
  }

  CompD* app = new CompD;
  (void)app;

  // Allow reset operations
  return true;
}

bool CompD::Helper::Reset() {
  std::cout << "Component-d: reset function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("d", false);
  Recorder::CountReset(init_level_);

  // Demonstrate delete of the owning singleton
  // allow operations to continue
  delete self_;
  return true;
}

// Static chain element
//
CompD::Helper CompD::helper_;
