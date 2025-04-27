#include <iostream>

template<template <typename> class TT>
class ContainerWrapper {
public:
  TT<int> value;
  ContainerWrapper(): value() {}
};

// 定义一个简单的模板类
template<typename T>
class SimpleContainer {
public:
  T data;

  SimpleContainer() : data{} {}
};

int main() {
  ContainerWrapper<SimpleContainer> wrapper;
  //模板张开
  /*
  class ContainerWrapper {
  public:
    SimpleContainer<int> value;
    ContainerWrapper() : value() {}
  }
  
  template<typename T>
  class SimpleContainer {
  public:
    T data;
    SimpleContainer() : data{} {}
  }
  */
  wrapper.value.data = 42;
  
  return 0;
}
