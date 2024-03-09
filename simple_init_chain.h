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

#if __cplusplus < 201103L
#error "At least c++11 is required"
#endif

namespace simple_init_chain {

// In order to have multiple independent chains
// it has to be a tagged template
//
// For simplicity there is also a default tag
//
struct DefaultTag {};

// The config map is used to pass arbitrary configuration
// information to init-worker instances

template <typename Tag = DefaultTag>
class InitChain {
 public:
  static bool Run(std::map<std::string, std::string> const& config_map =
                      std::map<std::string, std::string>()) {
    InitChain* cur = *GetList();

    bool ret = true;
    while (cur) {
      if (cur->init_done_ && !cur->multi_init_) {
        // Ignore duplicate init calls
        cur = cur->next_;
        continue;
      }

      // Just in case init worker deletes self
      // from inside InitFunc
      InitChain* tmp = cur->next_;

      cur->init_done_ = true;

      try {
        if (!cur->InitFunc(config_map)) {
          ret = false;
        }
      } catch (...) {
        ret = false;
      }
      cur = tmp;
    }
    return ret;
  }

  static void Reset(std::map<std::string, std::string> const& config_map =
                        std::map<std::string, std::string>()) {
    InitChain* cur = *GetList();

    while (cur) {
      // Prepare to handle init_done_
      // change on reset
      cur->init_done_copy_ = cur->init_done_;
      cur->init_done_ = false;

      // Just in case init worker deletes self
      // from inside ResetFunc
      InitChain* tmp = cur->next_;

      try {
        cur->ResetFunc(config_map);
      } catch (...) {
      }

      cur = tmp;
    }
  }

  virtual ~InitChain() noexcept {
    InitChain* prev = nullptr;
    InitChain* cur = *GetList();

    while (cur) {
      if (cur == this) {
        break;
      }
      prev = cur;
      cur = cur->next_;
    }

    if (!cur) {
      return;
    }

    if (prev == nullptr) {
      InitChain** list_ptr = GetList();
      *list_ptr = this->next_;
      return;
    }

    prev->next_ = this->next_;
    return;
  }

  // Allow to read back parameters and state
  //
  int GetLevel() const noexcept { return level_; }
  bool GetMultiInit() const noexcept { return multi_init_; }
  bool GetInitDone_() const noexcept { return init_done_; }

 protected:
  // level     - determines the order of execution
  //             lower values go first, coould be
  //             negative
  //
  // multi-init - if set every init will cause action
  //
  explicit InitChain(int level, bool multi_init = false) noexcept
      : next_(),
        init_done_(),
        init_done_copy_(),
        level_(level),
        multi_init_(multi_init) {
    InitChain* prev = nullptr;
    InitChain* cur = *GetList();

    while (cur) {
      if (level_ < cur->level_) {
        break;
      }
      prev = cur;
      cur = cur->next_;
    }

    this->next_ = cur;

    if (prev == nullptr) {
      InitChain** list_ptr = GetList();
      *list_ptr = this;
    } else {
      prev->next_ = this;
    }
  }

 private:
  // Function that actually does initialization, default is nop
  //
  virtual bool InitFunc(std::map<std::string, std::string> const&) {
    return true;
  }

  // Function that actually does reset, the default will keep
  // init_done_ unchanged
  //
  virtual void ResetFunc(std::map<std::string, std::string> const&) {
    // Keep init_done_ unchanged, restore it from saved copy
    init_done_ = init_done_copy_;
  }

  InitChain* next_;  // Next worker in the list
  bool init_done_;   // Init-done flag

  // If we did not derive the ResetFunc
  // we want to keep init_done_ value across
  // reset.
  //
  // If we did derive the ResetFunc we
  // want it to be cleared.
  //
  // Hence we have to keep this extra bit
  // of data
  bool init_done_copy_;
  int level_;        // Level
  bool multi_init_;  // Multi init support

 private:
  static InitChain** GetList() {
    static InitChain* the_list_;
    return &the_list_;
  }
};

}  // namespace simple_init_chain

#endif  // INCLUDE_SIMPLE_INIT_CHAIN_H_
