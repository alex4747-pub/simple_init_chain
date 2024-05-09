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

#ifndef TEST_COMMON_RECORDER_H_
#define TEST_COMMON_RECORDER_H_

#include <cstddef>
#include <map>
#include <string>

class Recorder final {
 public:
  // Count init and reset events per level
  static void CountInit(int level);
  static void CountReset(int level);

  // Get the level -> number-of-events map for inspection
  static std::map<int, size_t> const& GetInitMap() noexcept;
  static std::map<int, size_t> const& GetResetMap() noexcept;

  // Set/get state for component
  static void SetState(std::string const& name, bool);
  static size_t GetState(std::string const& name) noexcept;

  // Get Map of component states
  static std::map<std::string, size_t> const& GetStateMap() noexcept;

 private:
  static std::map<int, size_t> init_map_;
  static std::map<int, size_t> reset_map_;
  static std::map<std::string, size_t> state_map_;
};

#endif  // TEST_COMMON_RECORDER_H_
