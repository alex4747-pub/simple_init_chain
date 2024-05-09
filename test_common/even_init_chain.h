// Copyright (C) 2020  Aleksey Romanov (aleksey at voltanet dot io)Run
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

#ifndef TEST_COMMON_EVEN_INIT_CHAIN_H_
#define TEST_COMMON_EVEN_INIT_CHAIN_H_

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <mutex>  // NOLINT we need the standard mutex

#if __cplusplus < 201103L
#error "At least c++11 is required"
#endif

// This the default version. It places init chain code
// into the 'simple' namespace.
//
// Users can add additional independent chains by placing them
// into namespaces of their choice
//
// See README.md and comments in init_chain.inc for details
//

namespace even {

#define RUN_MUTEX_TYPEDEF using RUN_MUTEX = std::mutex;
#define LINK_MUTEX_TYPEDEF using LINK_MUTEX = std::mutex;

#include "init_chain.inc"

}  // namespace even

#endif  // TEST_COMMON_EVEN_INIT_CHAIN_H_
