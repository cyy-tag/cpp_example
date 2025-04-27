/*延迟实列化*/

/*1.部分和全部 实列化*/
//这里decltype只替换函数签名获取返回值，而没有替换函数体
template<typename T> T f(T p) { return 2*p; }
decltype(f(2)) x = 2;

//在不需要完全类型的(complete type) 情况下
//编译器也只实例化一部分
template<typename T> class Q {
  using Type = typename T::Type;
};

Q<int>* p = 0; //不会替换 Q<int> 的body
//尽管在这里int没有Type属性，但只替换声明部分并不会报错

int main()
{
  return 0;
}
