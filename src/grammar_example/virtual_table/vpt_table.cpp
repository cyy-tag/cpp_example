#include <iostream>
 
class Base {
public:
    virtual void fun() {
        std::cout << "Base::fun()" << std::endl;
    }
};
 
class Derived : public Base {
public:
    void fun() override {
        std::cout << "Derived::fun()" << std::endl;
    }
};
 
int main() {
    Derived* ptr = new Derived();
    typedef void(**vptr)(); //虚表指针类型
    
    // 获取对象的虚表指针, 虚表指针位置类实例头第一个字节
    vptr* p1 = (vptr*)ptr;
 
    // 通过虚表调用函数
    (**p1)(); //调用虚表第一个方法

    // (**p1) = 0; 不能修改虚函数表，虚函数表在段中，只读。
    delete ptr;
    return 0;
}
