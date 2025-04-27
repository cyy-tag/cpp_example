/*Injected Class Names(类名注入)*/
//在当前类的名字被当作如同是一个公开的成员名一样

#include <iostream>
#include <typeinfo>

void X() {
  std::cout << "called ::X()\n";
}

class X {
  public:
    static X create() {
      //在类内访问的, X指代注入类名
      return X();
    }

    static void invoke() {
      ::X(); //::调用查找全局作用域 X()函数
    }
};

int main() {
  std::cout << "Type create: " << typeid(X::create()).name() << std::endl;
  X::invoke();
  return 0;
}
