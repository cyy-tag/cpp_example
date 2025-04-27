//C++ template 在用到的时候进行实例化

template<typename T> class C; // 声明类模板C
C<int>* p = 0; //指针类型, 只需要声明，不需要定义

template<typename T>
class C {
public:
  C(int);
  void f(); // 模板类成员函数声明
};

void g(C<int>& c) // 引用类型，只需要声明
{
  c.f(); //使用模板类成员函数，需要在当前编译单元找到C::f()的定义
}

//C::f() 定义
template<typename T>
void C<T>::f()
{

}

//在重载决议中也会进行 实列化
void candidate(C<double>);
void candidate(int) {}

int main()
{
  candidate(42); //会重载candidate(int)
  //candidate(C<double>) 也会进行实列化判断是否符合重载, 因为都是单一参数
  //42可以隐式转换为C<double>右值作为传入参数
  //而重载遇到精确匹配不会选择其他需要隐式转换函数
  return 0;
}
