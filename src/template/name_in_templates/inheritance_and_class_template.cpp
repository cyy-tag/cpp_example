/**
 * Nondependent Base Classe: 基类是一个完全类型不会被类型模板参数影响
 * 在派生类中的类型的查找，非依赖基类会优先会模板参数列表
 */

//这里Base的完整类型不会受模板参数X的影响
template<typename X>
class Base {
public:
  int basefield_;
  using T = int;
};

class D1 : public Base<Base<void>> {
public:
  void f() { basefield_ = 3; } //访问的是基类成员
};

template<typename T>
class D2 : public Base<double> {
public:
  void f() { basefield_ = 7; } //访问基类成员
  T strange_; // T 是基类的类型Base<double>::T, 而不是typename T
};

/**
 * Dependent Base Classes
 */
//1.采用this访问基类成员变量，延迟查找
template<typename T>
class DD : public Base<T> { // Base基类需要依赖模板参数T
public:
  // void f() { basefield_ = 0; } 编译器报错: basefield_ was not declared in this scope
  //原因 Base<T> 依赖于模板参数T, 因此在查找是需要等到实例化模板
  void f() { this->basefield_ = 0; } //延迟名称查找(等待实例化后this查找)
};

//2.第二种方案
template<typename T>
class DD2 : public Base<T> {
public:
  void f() { Base<T>::basefield_ = 0; }
};

// template<> //显示特化
// class Base<bool> {
// public:
//   enum { basefield_ = 42 };
// };

void g(DD<bool>& d)
{
  d.f();
}

//在以下情况下第二种方案的延迟查找更合适
template<typename T>
class B {
public:
  enum E { e1 = 6, e2 = 28, e3 = 496 };
  virtual void zero(E e = e1);
  virtual void one(E&);
};

template<typename T>
class D : public B<T> {
public:
  void f() {
    typename D<T>::E e; //这里使用基类的类型名，而不是成员变量，不能使用this指针访问
  }
};

//3.使用using 方式延迟查找
template<typename T>
class DD3 : public Base<T> {
public:
  using Base<T>::basefield_;
  void f() { basefield_ = 0; } // fine
};

/**
 * 查找名称优先在当前实例化查找和全部非依赖模板参数基类中查找
 * 
 */
class NonDep {
  public:
    using Type = int;
};

template<typename T>
class Dep {
public:
  using OtherType = T;
};

template<typename T>
class DepBase : public NonDep, public Dep<T> {
public:
  void f() {
    typename DepBase<T>::Type t; //finds NonDep::Type;
    // DepBase<T>::OtherType ot = 1; // finds nothing; 循环依赖模板参数初始化失败
    // 内类查找循环依赖查找失败
  }
};

int main() {
  DepBase<int>::OtherType a = 1;  //ok.
  return 0;
}


