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

#ifndef TEST_COMMON_TEST_COMMON_H_
#define TEST_COMMON_TEST_COMMON_H_

// We have two ways of providing multiple
// chains: using namespace and template
// tags

// Namespace customization
#if defined(USE_NS_EVEN)

#include <even_init_chain.h>
#define INIT_CHAIN even::InitChain

#elif defined(USE_NS_ODD)

#include <odd_init_chain.h>
#define INIT_CHAIN odd::InitChain

#else

#if defined(USE_TAG_EVEN)

#include <even_tag.h>
#include <init_chain_tagged.h>
#define INIT_CHAIN simple::InitChain<Even>

#elif defined(USE_TAG_ODD)

#include <init_chain_tagged.h>
#include <odd_tag.h>
#define INIT_CHAIN simple::InitChain<Odd>

#else

#include <init_chain.h>
#define INIT_CHAIN simple::InitChain

#endif  // Tag customization
#endif  // Namesapce customization

#endif  // TEST_COMMON_TEST_COMMON_H_
