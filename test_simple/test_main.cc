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
#include <CompA.h>
#include <CompD.h>
#include <InitChain.h>
#include <Recorder.h>
#include <getopt.h>

#include <cassert>
#include <iostream>

static void usage() {
  std::cout << "usage: test_simple_init_chain [option]\n";
  std::cout << "where option could be:\n";
  std::cout << " -f,--failure        simulate operation failure\n";
  std::cout << " -e,--exception      throw exception from operation\n";
  std::cout << " -r,--release        do release\n";
  std::cout << " -l,--link-release   do release link\n";
}

// Static permssions
bool simple::InitChain::AllowReset() { return true; }

// Runner class
class TestRunner : public simple::InitChain::Runner {
 public:
  TestRunner() : Runner() {}

  TestRunner(TestRunner const& other) = default;
  TestRunner(TestRunner&& other) = default;
  TestRunner& operator=(TestRunner const& other) = default;
  TestRunner& operator=(TestRunner&& other) = default;

  bool Run() noexcept { return DoRun(); }
  bool Reset() noexcept { return DoReset(); }
  bool Release() noexcept { return DoRelease(); }
  bool Release(simple::InitChain::Link* link) noexcept {
    return DoRelease(link);
  }
};

int main(int argc, char** argv) {
  static struct option long_options[] = {
      {"exception", no_argument, 0, 1}, {"failure", no_argument, 0, 2},
      {"help", no_argument, 0, 3},      {"link-release", no_argument, 0, 4},
      {"release", no_argument, 0, 5},   {0, 0, 0, 0}};

  bool do_failure = false;
  bool do_exception = false;
  bool do_link_release = false;
  bool do_release = false;

  for (;;) {
    int c = getopt_long(argc, argv, "efhlr", long_options, 0);

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

      case 4:
      case 'l':
        if (do_release) {
          std::cout << "both link-release and release requested\n";
          usage();
          return 1;
        }
        do_link_release = true;
        break;

      case 5:
      case 'r':
        if (do_link_release) {
          std::cout << "both link-release and release requested\n";
          usage();
          return 1;
        }
        do_release = true;
        break;

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

  TestRunner test_runner;

  assert(Recorder::GetState("a") == 0);
  assert(Recorder::GetState("b") == 0);
  assert(Recorder::GetState("c") == 0);
  assert(Recorder::GetState("d") == 0);
  assert(Recorder::GetState("e") == 0);

  assert(Recorder::GetInitMap().size() == 0);
  assert(Recorder::GetResetMap().size() == 0);

  if (do_exception) {
    CompD::ArmException();

    auto res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 1);
    assert(Recorder::GetState("b") == 1);
    assert(Recorder::GetState("c") == 1);
    assert(Recorder::GetState("d") == 0);  // Exception
    assert(Recorder::GetState("e") == 2);

    // In UT environment we can re-run excepted entries
    res = test_runner.Reset();
    assert(res);

    assert(Recorder::GetState("a") == 1);  // No reset
    assert(Recorder::GetState("b") == 0);
    assert(Recorder::GetState("c") == 1);  // No reset
    assert(Recorder::GetState("d") == 0);  // Exception
    assert(Recorder::GetState("e") == 1);  // Two inits one reset

    res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 1);  // No reset, no additional init
    assert(Recorder::GetState("b") == 1);
    assert(Recorder::GetState("c") == 1);  // No reset no additonal init
    assert(Recorder::GetState("d") == 1);  // Retried after exception
    assert(Recorder::GetState("e") ==
           1);  // No more inits, both links deleted self

    assert(Recorder::GetInitMap().size() == 6);
    assert(Recorder::GetResetMap().size() == 3);
    return 0;
  }

  if (do_failure) {
    CompD::ArmFailure();
    auto res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 1);
    assert(Recorder::GetState("b") == 1);
    assert(Recorder::GetState("c") == 1);
    assert(Recorder::GetState("d") == 0);  // Failed
    assert(Recorder::GetState("e") == 2);

    // In UT environment we can re-run failed entries
    res = test_runner.Reset();
    assert(res);

    assert(Recorder::GetState("a") == 1);  // No reset
    assert(Recorder::GetState("b") == 0);
    assert(Recorder::GetState("c") == 1);  // No reset
    assert(Recorder::GetState("d") == 0);  // Failed
    assert(Recorder::GetState("e") == 1);  // Two inits one reset

    res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 1);  // No reset no additional init
    assert(Recorder::GetState("b") == 1);
    assert(Recorder::GetState("c") == 1);  // No reset no additional init
    assert(Recorder::GetState("d") == 1);  // Retried after exception
    assert(Recorder::GetState("e") ==
           1);  // No more inits, both links deleted self

    assert(Recorder::GetInitMap().size() == 6);
    assert(Recorder::GetResetMap().size() == 3);

    return 0;
  }

  if (do_link_release) {
    auto& link = CompA::GetLink();
    auto res = test_runner.Release(&link);
    assert(res);

    res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 0);  // Should be released
    assert(Recorder::GetState("b") == 1);
    assert(Recorder::GetState("c") == 1);
    assert(Recorder::GetState("d") == 1);
    assert(Recorder::GetState("e") == 2);

    assert(Recorder::GetInitMap().size() == 5);
    assert(Recorder::GetResetMap().size() == 0);

    return 0;
  }

  if (do_release) {
    auto res = test_runner.Release();
    assert(res);

    res = test_runner.Run();
    assert(res);

    assert(Recorder::GetState("a") == 0);
    assert(Recorder::GetState("b") == 0);
    assert(Recorder::GetState("c") == 0);
    assert(Recorder::GetState("d") == 0);
    assert(Recorder::GetState("e") == 0);

    assert(Recorder::GetInitMap().size() == 0);
    assert(Recorder::GetResetMap().size() == 0);
    return 0;
  }

  auto res = test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);
  assert(Recorder::GetState("b") == 1);
  assert(Recorder::GetState("c") == 1);
  assert(Recorder::GetState("d") == 1);
  assert(Recorder::GetState("e") == 2);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 0);

  // Duplicate calls are nops
  //
  res = test_runner.Run();
  assert(res);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 0);

  res = test_runner.Reset();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // Empty reset
  assert(Recorder::GetState("b") == 0);
  assert(Recorder::GetState("c") == 1);  // No reset
  assert(Recorder::GetState("d") == 0);
  // We have two instances of CompE one, deletes
  // self in init function and one in reset
  // We have two inits and one reset at the point
  assert(Recorder::GetState("e") == 1);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 3);

  res = test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // No reset, no new inits
  assert(Recorder::GetState("b") == 1);
  assert(Recorder::GetState("c") == 1);  // No reset, no new inits
  assert(Recorder::GetState("d") == 1);
  assert(Recorder::GetState("e") == 1);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 3);

  {
    auto const& init_map = Recorder::GetInitMap();

    for (auto cit = init_map.begin(); cit != init_map.end(); ++cit) {
      if ((*cit).first == -10) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 15) {
        assert((*cit).second == 2);
      } else if ((*cit).first == 20) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 25) {
        assert((*cit).second == 2);
      } else if ((*cit).first == 41) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 42) {
        assert((*cit).second == 1);
      } else {
        assert(false);
      }
    }

    auto const& reset_map = Recorder::GetResetMap();

    for (auto cit = reset_map.begin(); cit != reset_map.end(); ++cit) {
      if ((*cit).first == -10) {
        assert((*cit).second == 0);
      } else if ((*cit).first == 15) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 25) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 42) {
        assert((*cit).second == 1);
      } else {
        assert(false);
      }
    }
  }

  return 0;
}
