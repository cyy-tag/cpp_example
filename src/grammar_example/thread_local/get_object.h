#pragma once
#include <vector>
#include <unordered_map>


class Test
{
public:
  void TestFunc() {}
};


class Concurrence
{
public:
  Concurrence(){
    tests_[20] = Test();
  }
  void Run(uint16_t core_id) {
    tests_[core_id].TestFunc();
  }

private:
  std::unordered_map<uint16_t, Test> tests_;
};

class ThreadLocalConcurrence
{
public:
  
  void Run(uint16_t core_id) {
    test_.TestFunc();
  }

private:
  thread_local static Test test_;
};
