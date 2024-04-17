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

// This is the test component a.
//
#include <CompA.h>
#include <Recorder.h>

#include <cassert>
#include <iostream>
#include <string>

CompA* CompA::self_;

// Also, we are exercising usage of negative levels.
int const CompA::init_level_ = -10;

CompA& CompA::GetInstance() noexcept { return *self_; }

int CompA::GetCounter() noexcept { return counter_++; }

bool CompA::Check() noexcept { return true; }

CompA::CompA() noexcept : counter_() {}
CompA::~CompA() { self_ = nullptr; }

bool CompA::Init() {
  std::cout << "Component-a: init function called: level=" << init_level_
            << std::endl;
  Recorder::SetState("a", true);
  Recorder::CountInit(init_level_);

  // There is no reset-func so return value does not matter
  // there will be no further actions
  return true;
}

// Static instance of init_helper
//
INIT_CHAIN::Link CompA::init_helper_(init_level_, Init);
