#include <iostream>
//Argument-dependent-lookup
//对于函数查找, 不只在当前作用域查找，还可以根据提供的参数所对应的命名空间中进行查找
//选择最匹配的函数(参数类型和修饰一致)，进行重载

//缺点：对于没有写明作用域的函数，查找有可能会出现问题 例如
//using std::swap;
//swap(obj1, obj2); 如果在A::obj1, 命名空间A中存在A::swap(A::MyClass&, A::MyClass&)
//那么swap(obj1, obj2)调用的是命名空间中的swap

//采用全称调用 std::swap(obj1, obj2) 调用std::swap

namespace X
{
  template<typename T> void f(T);
} // namespace X

namespace N
{
 using namespace X;
 enum E { e1 }; 
 void f(E) {
    std::cout << "N::f(N::E) called\n";
 }
} // namespace N

void f(int)
{
  std::cout << "::f(int) called\n";
}

int main()
{
  ::f(N::e1);
  f(N::e1); //普通查找找到void f(int)
            //根据参数e1的作用域N查找到N::f(E), 后者更加匹配，调用后者
  return 0;
}
