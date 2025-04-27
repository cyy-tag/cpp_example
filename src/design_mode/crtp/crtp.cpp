#include <iostream>
#include <type_traits>

// 基类模板，使用递归模板模式
template <typename T>
class Base {
public:
    void print() {
        // std::cout << "Base print function" << std::endl;
        static_cast<T*>(this)->print();
    }
};

// 派生类
class Derived : public Base<Derived> {
public:
    void print() {
        std::cout << "Derived print function" << std::endl;
    }
};

int main() {
    Derived d;
    d.print(); // 调用Derived类的print函数

    Base<Derived>* basePtr = &d;
    basePtr->print(); // 通过基类指针调用，仍然调用Derived类的print函数

    return 0;
}
