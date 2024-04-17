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

#ifndef TEST_COMMON_COMPB_H_
#define TEST_COMMON_COMPB_H_

#include <TestCommon.h>

// A simple form of initialization: the chain-link object
// is a static member of the class.
//

// Application is a singleton
class CompB {
 public:
  CompB& GetInstance() noexcept;

  // Just an example of unrelated function
  int GetCounter() noexcept;

  // If shared library is used call to this function from main
  // will cause share library to be included into image by linker
  static bool Check() noexcept;

 private:
  CompB() noexcept;
  ~CompB();

  static bool Init();
  static bool Reset();

  int counter_;

  static CompB* self_;

  // We use levels for logging so it is a member of the class
  static int const init_level_;

  static INIT_CHAIN::Link init_helper_;
};

#endif  // TEST_COMMON_COMPB_H_
