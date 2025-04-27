#include <tuple>
#include <memory>
#include <iostream>

// Empty
class Foo { 
public:
  Foo() { std::cout << "Foo: "<< this <<std::endl;}
};

class Goo { 
public:
 Goo () { std::cout << "Goo: "<< this <<std::endl;}
};

// int main(int argc, char const *argv[]) {
//   Goo goo{};
//   Foo foo{};
//   auto t = std::make_tuple(100, foo, goo, 200);
//      std::get<int>(t)
//   std::cout << sizeof(t) << std::endl;
//   return 0;
// }

struct Empty {}; // 空类
struct X {
    [[no_unique_address]] Empty e;
};

int main() {
  X x;
  std::cout << sizeof(x) << std::endl;
  // 如何访问 x.e 的类型信息？
  std::cout << typeid(x.e).name() << std::endl; // 输出类型的信息
}
