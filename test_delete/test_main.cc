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
#include <getopt.h>
#include <simple_init_chain.h>

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::pair<std::string, std::string>> history_log;

static void DumpHistory(std::string const& heading) {
  std::cout << heading << std::endl;
  for(auto const& p: history_log) {
    std::cout << "    " << p.first << ": " << p.second << std::endl;
  }
}

static void ClearHistory() {
  history_log.clear();
}

static void CheckHistorySize(size_t size) {
  assert(history_log.size() == size);
}

class DynWorker : public simple_init_chain::InitChain<> {
 public:
  explicit DynWorker(int level, std::string const& name) :
      InitChain<>(level), name_(name),
      del_on_init_(), del_on_reset_() {}
  
  void ArmDeleteOnInit() { del_on_init_ = true; }
  void ArmDeleteOnReset() { del_on_reset_ = true; }
  
 private:
  
  bool InitFunc(std::map<std::string, std::string> const&) override {
    std::string del_string = (del_on_init_ ? " will delete self" : "");
    
    std::cout << "Component: " << name_ << " init function called: level=" << GetLevel()
              << del_string << std::endl;
    if (del_on_init_) {
      history_log.emplace_back(name_, "init-delete");
      delete this;
    } else {
      history_log.emplace_back(name_, "init");
    }
    return true;
  }
  
  void ResetFunc(std::map<std::string, std::string> const&) override {
    std::string del_string = (del_on_reset_ ? " will delete self" : "");
    
    std::cout << "Component: " << name_ << " reset function called: level=" << GetLevel()
              << del_string << std::endl;
    
    if (del_on_reset_) {
      history_log.emplace_back(name_, "reset-delete");
      delete this;
    } else {
      history_log.emplace_back(name_, "reset");
    }
  }

  std::string name_;
  bool del_on_init_;
  bool del_on_reset_;
};

static void usage() {
  std::cout << "usage: test_delete_init_chain\n";
}

int main(int argc, char**) {
  if (argc != 1) {
    std::cout << "unexpected parameters\n";
    usage();
    return 1;
  }
  
  // Create dynamic components
  auto comp_a = new DynWorker(10, "a");
  auto comp_b = new DynWorker(20, "b");
  auto comp_c = new DynWorker(30, "c");
  auto comp_d = new DynWorker(40, "d");
  auto comp_e = new DynWorker(50, "e");
  auto comp_f = new DynWorker(60, "f");
  auto comp_g = new DynWorker(70, "g");
  auto comp_h = new DynWorker(80, "h");

  (void)comp_d;
  (void)comp_e;
  
  // Initial Run  
  bool res = simple_init_chain::InitChain<>::Run();
  assert(res);

  CheckHistorySize(8);
  
  DumpHistory("Initial Run");
  ClearHistory();

  // Initial Reset
  simple_init_chain::InitChain<>::Reset();

  CheckHistorySize(8);

  DumpHistory("Initial Reset");
  ClearHistory();

  // Delete from middle
  comp_c->ArmDeleteOnInit();
  comp_c = nullptr; // About to delete self

  comp_f->ArmDeleteOnReset();
  comp_f = nullptr;

  res = simple_init_chain::InitChain<>::Run();
  assert(res);

  CheckHistorySize(8);
  
  DumpHistory("Delete from Middle Run");
  ClearHistory();

  simple_init_chain::InitChain<>::Reset();

  CheckHistorySize(7);

  DumpHistory("Delete from Middle Reset");
  ClearHistory();

  // Delete from start
  comp_a->ArmDeleteOnInit();
  comp_a = nullptr;

  comp_b->ArmDeleteOnReset();
  comp_b = nullptr;
  
  res = simple_init_chain::InitChain<>::Run();
  assert(res);

  CheckHistorySize(6);

  DumpHistory("Delete from Start Run");
  ClearHistory();

  simple_init_chain::InitChain<>::Reset();
  
  CheckHistorySize(5);
  
  DumpHistory("Delete from Start Reset");
  ClearHistory();

  // Delete from end
  comp_h->ArmDeleteOnInit();
  comp_h = nullptr;

  comp_g->ArmDeleteOnReset();
  comp_g = nullptr;
  
  res = simple_init_chain::InitChain<>::Run();
  assert(res);

  CheckHistorySize(4);

  DumpHistory("Delete from End Run");
  ClearHistory();

  simple_init_chain::InitChain<>::Reset();

  CheckHistorySize(3);

  DumpHistory("Delete from End Reset");
  ClearHistory();

  // Do final run to show final status
  res = simple_init_chain::InitChain<>::Run();
  assert(res);

  CheckHistorySize(2);

  DumpHistory("Final Run");
  ClearHistory();

  return 0;
}
