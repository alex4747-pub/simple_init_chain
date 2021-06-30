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

// NOLINT(build/header_guard)
//
#ifndef INCLUDE_SIMPLE_INIT_CHAIN_H_
#define INCLUDE_SIMPLE_INIT_CHAIN_H_

#include <cstddef>
#include <map>
#include <string>

#if __cplusplus < 201703L
#error "At least c++17 is required"
#endif

namespace simple {
class InitChain {
 public:
  // Just to save on typing
  //
  using ConfigMap = std::map<std::string, std::string>;

  static bool Run(ConfigMap const& config_map = ConfigMap()) {
    // Fail repeated calls or calls afer failure
    //
    if (!ready_) {
      return false;
    }
    ready_ = false;

    El* prev = nullptr;
    El* cur = the_list_;

    while (cur != nullptr) {
      // Make sure elements that do not support
      // reset are called only once
      //
      if (!cur->HaveReset()) {
        if (prev != nullptr) {
          // Skip current elemnt the next time
          //
          prev->SetNext(cur->GetNext());
        } else {
          // Discard leading element
          //
          the_list_ = the_list_->GetNext();
        }
        // Note we do not advance prev here
        //
      } else {
        prev = cur;
      }

      if (!cur->DoInit(config_map)) {
        // Only fatal failures are returned through this interface
        //
        ready_ = false;
        failed_ = true;
        return false;
      }

      cur = cur->GetNext();
    }

    return true;
  }

  // Reset initialization status
  // It is intended for unit-testing
  //
  static void Reset(ConfigMap const& config_map = ConfigMap()) {
    // Reset the run guard, prevent
    // further calls after failure
    //
    if (failed_) {
      return;
    }

    ready_ = true;

    for (El* cur = the_list_; cur != nullptr; cur = cur->GetNext()) {
      cur->DoReset(config_map);
    }
  }

  typedef bool (*InitFunc)(int level, ConfigMap const& config_map);
  typedef void (*ResetFunc)(int level, ConfigMap const& config_map);

  class El {
   public:
    El(int level, InitFunc init_func, ResetFunc reset_func = nullptr) noexcept
        : next_(),
          level_(level),
          init_func_(init_func),
          reset_func_(reset_func) {
      Insert(this);
    }

    ~El() noexcept {}

    int GetLevel() const noexcept { return level_; }

    El* GetNext() noexcept { return next_; }

    void SetNext(El* next) noexcept { next_ = next; }

    bool HaveReset() const noexcept { return reset_func_ != nullptr; }

    bool DoInit(ConfigMap const& config_map) noexcept {
      if (!init_func_) {
        return true;
      }

      try {
        return init_func_(level_, config_map);
      } catch (...) {
        return false;
      }
    }

    void DoReset(ConfigMap const& config_map) noexcept {
      if (!reset_func_) {
        return;
      }

      try {
        reset_func_(level_, config_map);
      } catch (...) {
      }
    }

   private:
    El* next_;
    int level_;
    InitFunc init_func_;
    ResetFunc reset_func_;
  };

 private:
  static void Insert(El* el) {
    el->SetNext(nullptr);

    El* prev = nullptr;
    El* cur = the_list_;

    while (cur != nullptr) {
      if (el->GetLevel() <= cur->GetLevel()) {
        el->SetNext(cur);
        break;
      }

      prev = cur;
      cur = cur->GetNext();
    }

    if (prev == nullptr) {
      the_list_ = el;
    } else {
      prev->SetNext(el);
    }
  }

  // Note: no danger of static initialization order
  // problem.
  static inline El* the_list_;

  // We expect that init operation will be
  // performed once.
  //
  // However, if  this code is executed in
  // a unit test environment it could be called
  // thousand times. Hence we have reset possibility.
  //
  // Also we do want to prevent repeated calls
  // after failure was declared.

  static inline bool ready_ = true;
  static inline bool failed_ = false;
};
}  // namespace simple

#endif  // INCLUDE_SIMPLE_INIT_CHAIN_H_
