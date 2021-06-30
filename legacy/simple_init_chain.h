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

// Let us make use of modern c++ functionality
// while being backward compatible with classic
//
// If NOEXCEPT is already defined hopefully it has
// correct value
//
#ifndef NOEXCEPT
#define NOEXCEPT_DEFINED 1
#if __cplusplus >= 201103L
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif
#endif

// If NULLPTR is already defined hopefully it has
// correct value
//
#ifndef NULLPTR
#define NULLPTR_DEFINED 1
#if __cplusplus >= 201103L
#define NULLPTR nullptr
#else
#define NULLPTR
#endif
#endif

namespace simple {
class InitChain {
 public:
  // Just to save on typing
  //
#if __cplusplus >= 201103L
  using ConfigMap = std::map<std::string, std::string>;
#else
  typedef std::map<std::string, std::string> ConfigMap;
#endif

  static bool Run(ConfigMap const& config_map = ConfigMap()) {
    // Faile repeated calls or calls afer failure
    //
    if (!RunGuard()) {
      return false;
    }

    El* prev = NULLPTR;
    El* cur = ListAccess();

    while (cur != NULLPTR) {
      // Make sure elements that do not support
      // reset are called only once
      //
      if (!cur->HaveReset()) {
        if (prev != NULLPTR) {
          // Skip current elemnt the next time
          //
          prev->SetNext(cur->GetNext());
        } else {
          // Discard leading element
          //
          ListAccess(NULLPTR, true);
        }
        // Note we do not advance prev here
        //
      } else {
        prev = cur;
      }

      if (!cur->DoInit(config_map)) {
        // Only fatal failures are returned through this interface
        //
        RunGuard(kFailure);
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
    // further callse after failure
    //
    if (!RunGuard(kReset)) {
      return;
    }

    for (El* cur = ListAccess(); cur != NULLPTR; cur = cur->GetNext()) {
      cur->DoReset(config_map);
    }
  }

  typedef bool (*InitFunc)(int level, ConfigMap const& config_map);
  typedef void (*ResetFunc)(int level, ConfigMap const& config_map);

  class El {
   public:
    El(int level, InitFunc init_func, ResetFunc reset_func = NULLPTR) NOEXCEPT
        : next_(),
          level_(level),
          init_func_(init_func),
          reset_func_(reset_func) {
      ListAccess(this);
    }

    ~El() NOEXCEPT {}

    int GetLevel() const NOEXCEPT { return level_; }

    El* GetNext() NOEXCEPT { return next_; }

    void SetNext(El* next) NOEXCEPT { next_ = next; }

    bool HaveReset() const NOEXCEPT { return reset_func_ != NULLPTR; }

    bool DoInit(ConfigMap const& config_map) NOEXCEPT {
      if (!init_func_) {
        return true;
      }

      try {
        return init_func_(level_, config_map);
      } catch (...) {
        return false;
      }
    }

    void DoReset(ConfigMap const& config_map) NOEXCEPT {
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
  // We expect that init operation will be
  // performed once.
  //
  // However, if  this code is executed in
  // a unit test environment it could be called
  // thousand times. Hence we have reset possibility.
  //
  // Also we do want to prevent repeated calls
  // after failure was declared.
  //
  // In order to be include-only component for pre c++17
  // compilers the list has to be resrepsented by a funciton
  //

  enum RunGuardOp { kRead = 0, kReset, kFailure };

  static bool RunGuard(RunGuardOp op = kRead) NOEXCEPT {
    static bool value = true;
    static bool failed = false;

    if (failed) {
      return false;
    }

    switch (op) {
      case kRead:
        if (value) {
          value = false;
          return true;
        }
        return false;

      case kReset:
        value = true;
        break;

      case kFailure:
        failed = true;
        value = false;
        break;
    }

    return value;
  }

  // In order to be include-only component for pre c++17
  // compilers the list has to be resrepsented by a funciton
  //
  static El* ListAccess(El* el = NULLPTR, bool discard_head = false) NOEXCEPT {
    static El* the_list = NULLPTR;

    if (el == NULLPTR) {
      if (discard_head && the_list != NULLPTR) {
        the_list = the_list->GetNext();
      }
      return the_list;
    }

    el->SetNext(NULLPTR);

    El* prev = NULLPTR;
    El* cur = the_list;

    while (cur != NULLPTR) {
      if (el->GetLevel() <= cur->GetLevel()) {
        el->SetNext(cur);
        break;
      }

      prev = cur;
      cur = cur->GetNext();
    }

    if (prev == NULLPTR) {
      the_list = el;
    } else {
      prev->SetNext(el);
    }

    return the_list;
  }
};
}  // namespace simple

// If NOEXCEPT was defined in this
// file, clean it up
//
#ifdef NOEXCEPT_DEFINED
#undef NOEXCEPT
#undef NOEXCEPT_DEFINED
#endif

// If NULLPTR was defined in this
// file, clean it up
//
#ifdef NULLPTR_DEFINED
#undef NULLPTR
#undef NULLPTR_DEFINED
#endif

#endif  // INCLUDE_SIMPLE_INIT_CHAIN_H_
