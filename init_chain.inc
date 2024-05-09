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

// This file may be included multiple times
// no scope protectors

// Add missing includes to make tidy happy
#ifdef RUNNING_CPP_TIDY

#include <functional>
#include <mutex>  // NOLINT we need the standard mutex

#if RUNNING_CPP_TIDY == 2

// Special case for tagged environment
//
#define RUN_MUTEX_TYPEDEF
#define LINK_MUTEX_TYPEDEF

#else

#define RUN_MUTEX_TYPEDEF using RUN_MUTEX = std::mutex;
#define LINK_MUTEX_TYPEDEF using LINK_MUTEX = std::mutex;

#endif
#endif

class InitChain {
 public:
  RUN_MUTEX_TYPEDEF
  LINK_MUTEX_TYPEDEF

  // Chain link class
  class Link {
   public:
    Link() = delete;
    Link(Link const& other) = delete;
    Link(Link&& other) = delete;

    // level       - determines the order of init execution
    //               lower values go first, could be negative
    //  init_func  - init function, returns false if no further
    //               resets should be scheduled
    //  reset_func - optional reset function, returns false if no further
    //               inits should be scheduled
    explicit Link(int level, std::function<bool()> init_func,
                  std::function<bool()> reset_func = nullptr) noexcept
        : next_(),
          prev_(),
          in_list_(),
          level_(level),
          init_func_(init_func),
          reset_func_(reset_func) {
      if (!init_func_) abort();
      Bucket* bucket = GetBucket();
      std::lock_guard<std::mutex> guard(bucket->link_mutex);
      if (bucket->link_lock) {
        return;
      }
      Insert(this, &bucket->init_list, true);
    }

    virtual ~Link() {
      Bucket* bucket = GetBucket();
      std::lock_guard<std::mutex> guard(bucket->link_mutex);

      if (this == bucket->active_link) {
        // Being deleted while being processed
        bucket->active_link = nullptr;
      }

      Remove(this, &bucket->init_list, &bucket->reset_list);
    }

    Link& operator=(Link const& other) = delete;
    Link& operator=(Link&& other) = delete;

    int GetLevel() const noexcept { return level_; }
    bool IsInList() const noexcept { return in_list_; }

   private:
    // Class data
    Link* next_;    // Next chain in the list
    Link* prev_;    // Prev worker in the list
    bool in_list_;  // Inserted in a list
    int level_;     // Level
    std::function<bool()> init_func_;
    std::function<bool()> reset_func_;

    friend class InitChain;
  };

  /////////////////////////////////////////////////////////
  // Runner class
  class Runner {
   public:
    virtual ~Runner() {}

   protected:
    Runner() noexcept {}
    Runner(Runner const& other) = default;
    Runner(Runner&& other) = default;
    Runner& operator=(Runner const& other) = default;
    Runner& operator=(Runner&& other) = default;

    bool DoRun() noexcept { return InitChain::Run(); }
    bool DoReset() noexcept { return InitChain::Reset(); }
    bool DoRelease() noexcept { return InitChain::Release(); }
    bool DoRelease(InitChain::Link* link) noexcept {
      return InitChain::Release(link);
    }
  };

  ///////////////////////////////////////////////////////////
  // Global configuration

  // Global configuration as non-inline static functions
  // they are called once on the first run.
  //
  // Using non-inlined static functions serves three purposes
  //
  // 1. Validate that there is one and only one configuration
  //    preferably located right next to main.
  //
  // 2. Provides a visual reminder that Run() function
  //    should be called.
  //
  // 3. Provides a convenient way to determine the global
  //    configuration at compile time of a single unit.
  //    So it easy to have UT and non-UT configs
  //

  // Allow resets
  static bool AllowReset();

 private:
  ///////////////////////////////////////////////
  // Helper functions

  static Link* Pop(Link** list_ptr) noexcept {
    Link* head = *list_ptr;

    if (!head) {
      return nullptr;
    }

    *list_ptr = head->next_;
    if (*list_ptr) {
      (*list_ptr)->prev_ = nullptr;
    }

    head->next_ = nullptr;
    head->prev_ = nullptr;
    head->in_list_ = false;
    return head;
  }

  static void Insert(Link* link, Link** list_ptr, bool ascending) noexcept {
    if (!list_ptr) abort();

    Link* prev = nullptr;
    Link* cur = *list_ptr;

    if (ascending) {
      while (cur && cur->level_ <= link->level_) {
        prev = cur;
        cur = cur->next_;
      }
    } else {
      while (cur && cur->level_ > link->level_) {
        prev = cur;
        cur = cur->next_;
      }
    }

    link->prev_ = prev;
    if (prev) {
      prev->next_ = link;
    } else {
      *list_ptr = link;
    }

    link->next_ = cur;
    if (cur) {
      cur->prev_ = link;
    }
    link->in_list_ = true;
    return;
  }

  static void Remove(Link* link, Link** init_listptr, Link** reset_listptr) {
    if (!link->in_list_) {
      return;
    }

    if (link->next_) {
      link->next_->prev_ = link->prev_;
    }

    if (link->prev_) {
      link->prev_->next_ = link->next_;
    } else if (link == *init_listptr) {
      *init_listptr = link->next_;
    } else if (link == *reset_listptr) {
      *reset_listptr = link->next_;
    } else {
      abort();
    }

    link->next_ = nullptr;
    link->prev_ = nullptr;
    link->in_list_ = false;
  }

  /////////////////////////////////////////////////////////////////////////
  // Top level opeations

  // Run initialization for all chain-links in init
  // chain
  //
  // Loop through the init chain: remove head chain-link and
  // call init(), if resets are allowed push the entry into
  // reset chain.
  //
  // Returns: success/failure, the only reason for failure
  // if run-mutex was locked

  static bool Run() noexcept {
    Bucket* bucket = GetBucket();
    std::unique_lock<std::mutex> run_guard(bucket->run_mutex, std::try_to_lock);

    if (!run_guard.owns_lock()) {
      return false;
    }

    // Read config on the first init
    if (!bucket->activated) {
      bucket->activated = true;
      bucket->reset_ok = AllowReset();
    }

    for (;;) {
      Link* cur = nullptr;
      {
        std::lock_guard<std::mutex> guard(bucket->link_mutex);
        cur = Pop(&bucket->init_list);
        bucket->active_link = cur;
      }

      if (!cur) {
        break;
      }

      // Execute init function, return true if resets are ok
      // from its point of view
      bool res = true;
      if (cur->init_func_) {
        try {
          res = cur->init_func_();
        } catch (...) {
          // So far we allow inits that threw an
          // exception to be reset and retried
        }
      }

      std::lock_guard<std::mutex> guard(bucket->link_mutex);

      if (!cur->reset_func_ || !res || !bucket->reset_ok ||
          !bucket->active_link) {
        // No reset function, init function returned false, or resets are not
        // allowed, or active entry was deleted inside the init call: nothing to
        // do
        //
        bucket->active_link = nullptr;  // For consistency sake
        continue;
      }

      bucket->active_link = nullptr;  // For consistency sake

      if (!cur->reset_func_) {
        // No reset function provided
        continue;
      }

      // Insert processed entry into reset list, in most
      // cases there wil be no list walk involved
      Insert(cur, &bucket->reset_list, false);
    }

    return true;
  }

  // Run resets for all chain-links in reset chain
  // (the order will be reverse to order of initialization).
  //
  // Loop through the reset chain: remove head chain-link and
  // call reset(), in case of unlimited extensions push the entry
  // to the init chain.
  //
  // Returns: success failure, the only reason of failure if
  // run-mutex was locked
  //

  static bool Reset() noexcept {
    Bucket* bucket = GetBucket();

    std::unique_lock<std::mutex> run_guard(bucket->run_mutex, std::try_to_lock);

    if (!run_guard.owns_lock()) {
      return false;
    }

    if (!bucket->activated) {
      // Nothing to do yet, consider it success
      return true;
    }

    if (!bucket->reset_ok) {
      // Resets are not enabled consider
      // it success
      return true;
    }

    for (;;) {
      Link* cur = nullptr;
      {
        std::lock_guard<std::mutex> guard(bucket->link_mutex);
        cur = Pop(&bucket->reset_list);
        bucket->active_link = cur;
      }

      if (!cur) {
        break;
      }

      bool res = false;
      if (cur->reset_func_) {
        try {
          res = cur->reset_func_();
        } catch (...) {
        }
      }

      std::lock_guard<std::mutex> guard(bucket->link_mutex);
      if (!res || !bucket->active_link) {
        // There is no reset function, or it returned false,
        // or excepted, or the active entry was deleted inside
        // the reset call: nothing to do
        //
        bucket->active_link = nullptr;  // For consistency sake
        continue;
      }

      bucket->active_link = nullptr;  // For consistency sake

      // Insert processed entry into init list, in most
      // cases there wil be no list walk involved
      Insert(cur, &bucket->init_list, true);
    }

    return true;
  }

  // Sets link_lock flag and releases all links form all lists
  //
  // Returns: success/failure, the only reason for failure
  // if run-mutex was locked
  static bool Release() noexcept {
    Bucket* bucket = GetBucket();

    std::unique_lock<std::mutex> run_guard(bucket->run_mutex, std::try_to_lock);

    if (!run_guard.owns_lock()) {
      return false;
    }

    std::lock_guard<std::mutex> guard(bucket->link_mutex);

    bucket->link_lock = true;

    Link* cur = bucket->init_list;

    while (cur) {
      Link* next = cur->next_;

      cur->prev_ = nullptr;
      cur->next_ = nullptr;
      cur->in_list_ = false;

      cur = next;
    }

    bucket->init_list = nullptr;

    cur = bucket->reset_list;

    while (cur) {
      Link* next = cur->next_;

      cur->prev_ = nullptr;
      cur->next_ = nullptr;
      cur->in_list_ = false;

      cur = next;
    }

    bucket->reset_list = nullptr;
    return true;
  }

  // Release single link, suposedly to be deleted

  // Returns: success/failure, the only reason for failure
  // if run-mutex was locked
  static bool Release(Link* link) noexcept {
    if (!link) {
      return true;
    }

    Bucket* bucket = GetBucket();

    std::unique_lock<std::mutex> run_guard(bucket->run_mutex, std::try_to_lock);

    if (!run_guard.owns_lock()) {
      return false;
    }

    std::lock_guard<std::mutex> guard(bucket->link_mutex);

    if (!link->in_list_) {
      return true;
    }

    Remove(link, &bucket->init_list, &bucket->reset_list);
    return true;
  }

  // Bucket to keep the collection of static data
  //
  struct Bucket {
    // Run mutex provides mutual exclusion between Runs(), Reset(),
    // and Release(), also protects activate field
    RUN_MUTEX run_mutex;

    // Set true on the first Run(), used to read
    // config
    bool activated;

    // Configuration
    bool reset_ok;

    // Link mutex protects access to link data
    LINK_MUTEX link_mutex;

    // Link currently in process
    Link* active_link;

    // Init list
    Link* init_list;

    // Reset list
    Link* reset_list;

    // Constructors would not link self into init list
    bool link_lock;
  };

  // Static operaton primitives
  static Bucket* GetBucket() noexcept {
    static Bucket chain_bucket;
    return &chain_bucket;
  }

  // Used by tagged version to force explicit
  // specialization of config functions
  static constexpr bool allow_helper() { return false; }
};

#undef RUN_MUTEX_TYPEDEF
#undef LINK_MUTEX_TYPEDEF
