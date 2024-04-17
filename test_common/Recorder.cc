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
#include <Recorder.h>

#include <utility>

std::map<int, size_t> Recorder::init_map_;
std::map<int, size_t> Recorder::reset_map_;
std::map<std::string, size_t> Recorder::state_map_;

void Recorder::CountInit(int level) {
  auto res = init_map_.insert(std::make_pair(level, 1));

  if (!res.second) {
    (*res.first).second++;
  }
}

void Recorder::CountReset(int level) {
  auto res = reset_map_.insert(std::make_pair(level, 1));

  if (!res.second) {
    (*res.first).second++;
  }
}

std::map<int, size_t> const& Recorder::GetInitMap() noexcept {
  return init_map_;
}
std::map<int, size_t> const& Recorder::GetResetMap() noexcept {
  return reset_map_;
}

void Recorder::SetState(std::string const& name, bool val) {
  if (val) {
    auto res = state_map_.insert(std::make_pair(name, 1));
    if (!res.second) {
      (*res.first).second++;
    }
  } else {
    auto cit = state_map_.find(name);
    if (cit != state_map_.end() && (*cit).second > 0) {
      (*cit).second--;
    }
  }
}

size_t Recorder::GetState(std::string const& name) noexcept {
  auto cit = state_map_.find(name);
  if (cit == state_map_.end()) {
    return 0;
  }
  return (*cit).second;
}
