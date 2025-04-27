#include <iostream>
#include <iostream>
// std::remove_reference_t 是 C++11 标准库中的一个模板别名
// 它是 std::remove_reference 模板的特化版本，用于去除类型中的引用部分。
// template <class T>
// struct remove_reference { using type = T; };

// template <class T>
// struct remove_reference<T&> { using type = T; };

// template <class T>
// struct remove_reference<T&&> { using type = T; };

// template <class T>
// using remove_reference_t = typename remove_reference<T>::type;

int main(int argc, char*argv[])
{
  //typeid(a).name() 返回的是绑定对象的信息, 引用和非引用返回一致
  // 非引用类型
  std::remove_reference_t<int> a = 10;
  std::cout << "a is of type: " << typeid(a).name() << std::endl;


  //左值引用
  int & b = a;
  std::remove_reference_t<int &> rb = 20;
  std::cout << "lr: " << typeid(b).name() << std::endl;
  std::cout << "rb: " << typeid(rb).name() << std::endl;

  //右值引用
  int&& c = 30;
  std::remove_reference_t<int&&> rc = 30;
   std::cout << "rc: " << typeid(int&&).name() << std::endl;
  std::cout << "rc1: " << typeid(rc).name() << std::endl; 
  return 0;
}
