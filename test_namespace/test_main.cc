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
#include <EvenInitChain.h>
#include <OddInitChain.h>
#include <Recorder.h>
#include <getopt.h>

#include <cassert>
#include <iostream>

// Permissions, separate for each namespace
bool even::InitChain::AllowReset() { return true; }

bool odd::InitChain::AllowReset() { return true; }

// Runner classes
class EvenTestRunner : public even::InitChain::Runner {
 public:
  EvenTestRunner() : Runner() {}

  EvenTestRunner(EvenTestRunner const& other) = default;
  EvenTestRunner(EvenTestRunner&& other) = default;
  EvenTestRunner& operator=(EvenTestRunner const& other) = default;
  EvenTestRunner& operator=(EvenTestRunner&& other) = default;

  bool Run() noexcept { return DoRun(); }
  bool Reset() noexcept { return DoReset(); }
  bool Release() noexcept { return DoRelease(); }
};

class OddTestRunner : public odd::InitChain::Runner {
 public:
  OddTestRunner() : Runner() {}

  OddTestRunner(OddTestRunner const& other) = default;
  OddTestRunner(OddTestRunner&& other) = default;
  OddTestRunner& operator=(OddTestRunner const& other) = default;
  OddTestRunner& operator=(OddTestRunner&& other) = default;

  bool Run() noexcept { return DoRun(); }
  bool Reset() noexcept { return DoReset(); }
  bool Release() noexcept { return DoRelease(); }
};

int main(int argc, char**) {
  if (argc != 1) {
    std::cout << "unexpected parameters\n";
    return 1;
  }

  EvenTestRunner even_test_runner;
  OddTestRunner odd_test_runner;

  assert(Recorder::GetState("a") == 0);
  assert(Recorder::GetState("b") == 0);
  assert(Recorder::GetState("c") == 0);
  assert(Recorder::GetState("d") == 0);
  assert(Recorder::GetState("e") == 0);

  assert(Recorder::GetInitMap().size() == 0);
  assert(Recorder::GetResetMap().size() == 0);

  auto res = even_test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);
  assert(Recorder::GetState("b") == 0);
  assert(Recorder::GetState("c") == 1);
  assert(Recorder::GetState("d") == 0);
  assert(Recorder::GetState("e") == 2);

  assert(Recorder::GetInitMap().size() == 4);
  assert(Recorder::GetResetMap().size() == 0);

  // Duplicate calls should be nops
  //
  res = even_test_runner.Run();
  assert(res);

  assert(Recorder::GetInitMap().size() == 4);
  assert(Recorder::GetResetMap().size() == 0);

  res = odd_test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);
  assert(Recorder::GetState("b") == 1);
  assert(Recorder::GetState("c") == 1);
  assert(Recorder::GetState("d") == 1);
  assert(Recorder::GetState("e") == 2);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 0);

  // Duplicate calls should be nops
  //
  res = odd_test_runner.Run();
  assert(res);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 0);

  res = even_test_runner.Reset();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // Empty reset
  assert(Recorder::GetState("b") == 1);  // Not called
  assert(Recorder::GetState("c") == 1);  // No reset
  assert(Recorder::GetState("d") == 1);  // Not caled
  // We have two instances of CompE one, deletes
  // self in init function and one in reset
  // We have two inits and one reset at the point
  assert(Recorder::GetState("e") == 1);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 1);

  res = odd_test_runner.Reset();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // Empty reset
  assert(Recorder::GetState("b") == 0);  // Reset done
  assert(Recorder::GetState("c") == 1);  // No reset
  assert(Recorder::GetState("d") == 0);  // Reset done
  assert(Recorder::GetState("e") == 1);  // No reset

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 3);

  res = even_test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // No reset, no new inits
  assert(Recorder::GetState("b") == 0);  // No call
  assert(Recorder::GetState("c") == 1);  // No reset, no new inits
  assert(Recorder::GetState("d") == 0);  // No call
  assert(Recorder::GetState("e") == 1);  // Instanced deleted

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 3);

  res = even_test_runner.Run();
  assert(res);

  assert(Recorder::GetState("a") == 1);  // No reset, no new inits
  assert(Recorder::GetState("b") == 0);  // No call
  assert(Recorder::GetState("c") == 1);  // No reset, no new inits
  assert(Recorder::GetState("d") == 0);  // No call
  assert(Recorder::GetState("e") == 1);

  assert(Recorder::GetInitMap().size() == 6);
  assert(Recorder::GetResetMap().size() == 3);

  {
    auto const& init_map = Recorder::GetInitMap();

    for (auto cit = init_map.begin(); cit != init_map.end(); ++cit) {
      if ((*cit).first == -10) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 15) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 20) {
        assert((*cit).second == 1);
      } else if ((*cit).first == 25) {
        assert((*cit).second == 1);
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
      } else if ((*cit).first == 41) {
        assert((*cit).second == 0);
      } else if ((*cit).first == 42) {
        assert((*cit).second == 1);
      } else {
        assert(false);
      }
    }
  }

  return 0;
}
