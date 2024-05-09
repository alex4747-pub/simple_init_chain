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

#ifndef TEST_COMMON_COMP_D_H_
#define TEST_COMMON_COMP_D_H_

// D-component is a special case we can arm failures
// or exception.
//
// We use static member of class it is preferred way
// preventing multiple chain links to be initializing
// the same class

// The application is a singleton
class CompD {
 public:
  CompD& GetInstance() noexcept;

  // Just an example of unrelated function
  int GetCounter() noexcept;

  // Component D does not have Check function
  // it will be explicitly loaded by dlopen.

  static void ArmFailure() noexcept;
  static void ArmException() noexcept;

 private:
  CompD() noexcept;
  ~CompD();

  int counter_;

  static CompD* self_;
  static bool failure_armed_;
  static bool exception_armed_;

  // We use levels for logging so it is a member of the class
  static int const init_level_;

  // Initialization helper class
  class Helper;

  // Static instance of helper class
  static Helper helper_;
};

#endif  // TEST_COMMON_COMP_D_H_
