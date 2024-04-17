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

#ifndef TEST_COMMON_COMPC_H_
#define TEST_COMMON_COMPC_H_

// This example demonstrates several more variants:
//
// 1. We derive from the ChainLink and use it as a
//    static member: in examples a and b we used base
//    ChainLink
//
// 2. The derived class is forward declared and non-public,
//    we do not need neither forward declaration of the
//    ChainLink nor include of the ChainLink.h
//
// 3. We pass member functions of the derived class
//    to the base ChainLink: in examples a and be we
//    passed static functons
//
// 4. We also demostrate inclusion of Reset functionality
//    by compile time #define

class CompC {
 public:
  CompC& GetInstance() noexcept;

  // Just an example of unrelated function
  int GetCounter() noexcept;

  // If shared library is used call to this function from main
  // will cause share library to be included into image by linker
  static bool Check() noexcept;

 private:
  CompC() noexcept;
  ~CompC();

  int counter_;

  static CompC* self_;

  // We use levels for logging so it is a member of the class
  static int const init_level_;

  class InitHelper;

  static InitHelper init_helper_;
};

#endif  // TEST_COMMON_COMPC_H_
