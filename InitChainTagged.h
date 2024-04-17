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

#ifndef INITCHAINTAGGED_H_
#define INITCHAINTAGGED_H_

#include <functional>
#include <mutex>  // NOLINT we need the standard mutex

#if __cplusplus < 201103L
#error "At least c++11 is required"
#endif

// This version uses template tag type to support
// multiple chains
//
// See README.md and comments in InitChain.inl for
// detais
//

namespace simple {

// Tagged initialization chain
//

// No need in typedefs when we use templates
//
#define RUN_MUTEX_TYPEDEF
#define LINK_MUTEX_TYPEDEF

template <typename TAG, typename RUN_MUTEX = std::mutex,
          typename LINK_MUTEX = std::mutex>
#include "InitChain.inl"

template <typename TAG, typename RUN_MUTEX, typename LINK_MUTEX>
bool InitChain<TAG, RUN_MUTEX, LINK_MUTEX>::AllowReset() {
  static_assert(allow_helper(), "use specialised config");
  return false;
}

}  // namespace simple

#endif  // INITCHAINTAGGED_H_
