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
#include <simple_init_chain.h>
#include <test_comp_a.h>
#include <test_comp_b.h>
#include <test_comp_c.h>
#include <test_comp_d.h>
#include <test_comp_e.h>
#include <test_counter.h>

#include <getopt.h>
#include <cassert>
#include <iostream>

static void usage() {
  std::cout << "usage: test_simple_init_chain [{--exception | --failure}]\n"
            << "   or  test_simple_init_chain [{ -e | -f }]\n";
}

int main(int argc, char** argv) {
  static struct option long_options[] = {{"exception", no_argument, 0, 1},
                                         {"failure", no_argument, 0, 2},
                                         {"help", no_argument, 0, 3},
                                         {0, 0, 0, 0}};

  bool do_failure = false;
  bool do_exception = false;

  for (;;) {
    int c = getopt_long(argc, argv, "efh", long_options, 0);

    if (c < 0) {
      break;
    }

    switch (c) {
      case 1:
      case 'e':
        if (do_failure) {
          std::cout << "both failure and exception requested\n";
          usage();
          return 1;
        }

        do_exception = true;
        break;

      case 2:
      case 'f':
        if (do_exception) {
          std::cout << "both failure and exception requested\n";
          usage();
          return 1;
        }

        do_failure = true;
        break;

      case 3:
      case 'h':
        usage();
        return 0;

      default:
        usage();
        return 1;
    }
  }

  if (optind != argc) {
    std::cout << "unexpected parameters\n";
    usage();
    return 1;
  }

  if (do_exception) {
    ArmDException();

    bool res = simple::InitChain::Run();
    assert(!res);

    // We cannot reset a failed run
    //
    simple::InitChain::Reset();

    res = simple::InitChain::Run();
    assert(!res);

    return 0;
  }

  if (do_failure) {
    ArmDFailure();

    bool res = simple::InitChain::Run();
    assert(!res);

    // We cannot reset a failed run
    //
    simple::InitChain::Reset();

    res = simple::InitChain::Run();
    assert(!res);

    return 0;
  }

  assert(!GetCompAState());
  assert(!GetCompBState());
  assert(!GetCompCState());
  assert(!GetCompDState());
  assert(!GetCompEState());

  assert(GetInitMap().size() == 0);
  assert(GetResetMap().size() == 0);

  bool res = simple::InitChain::Run();
  assert(res);

  assert(GetCompAState());
  assert(GetCompBState());
  assert(GetCompCState());
  assert(GetCompDState());
  assert(GetCompEState());

  assert(GetInitMap().size() == 5);
  assert(GetResetMap().size() == 0);

  // Duplicate call should fail immediately
  //
  res = simple::InitChain::Run();
  assert(!res);

  assert(GetInitMap().size() == 5);
  assert(GetResetMap().size() == 0);

  simple::InitChain::Reset();

  assert(GetCompAState());
  assert(!GetCompBState());
  assert(GetCompCState());
  assert(!GetCompDState());
  assert(GetCompEState());

  assert(GetInitMap().size() == 5);
  assert(GetResetMap().size() == 2);

  res = simple::InitChain::Run();
  assert(res);

  assert(GetCompAState());
  assert(GetCompBState());
  assert(GetCompCState());
  assert(GetCompDState());
  assert(GetCompEState());

  assert(GetInitMap().size() == 5);
  assert(GetResetMap().size() == 2);

  {
    CountMap const& init_map = GetInitMap();

    for (CountMap::const_iterator cit = init_map.begin(); cit != init_map.end();
         ++cit) {
      if ((*cit).first == -10) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 15) {
        assert((*cit).second == 2);
      } else if ((*cit).first == 20) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 25) {
        assert((*cit).second == 2);
      } else if ((*cit).first == 30) {
        assert((*cit).second == 1);
      } else {
        assert(false);
      }
    }

    CountMap const& reset_map = GetResetMap();

    for (CountMap::const_iterator cit = reset_map.begin();
         cit != reset_map.end(); ++cit) {
      if ((*cit).first == -10) {
        assert((*cit).second == 0);
      } else if ((*cit).first == 15) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 20) {
        assert((*cit).second == 0);
      } else if ((*cit).first == 25) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 30) {
        assert((*cit).second == 0);
      } else {
        assert(false);
      }
    }
  }

  return 0;
}
