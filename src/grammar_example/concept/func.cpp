#include <concepts>
#include <iostream>
#include <type_traits>

// 概念：检查类型T是否有一个接受特定类型ExpectedArgType参数的成员函数func
template<typename T, typename ExpectedArgType>
concept HasFuncWithSpecificArgType = requires(T t, ExpectedArgType arg)
{
    // 检查t.func(arg)是否可以调用，并且参数类型必须是ExpectedArgType
    { t.func(arg) } -> std::same_as<void>;
    requires std::same_as<ExpectedArgType, std::remove_cvref_t<decltype(arg)>>;
};

class A
{
public:
    void func(int x) { std::cout << "A::func(int) called with value: " << x << std::endl; }
};

// 使用概念作为模板函数的约束
// 这里我们也指定了参数类型Arg必须为int
template<HasFuncWithSpecificArgType T, typename Arg>
requires std::same_as<Arg, int>
void useTypeWithFunc(T t)
{
    t.func(arg); // 调用成员函数func，概念保证了T有接受int类型参数的func
}

int main()
{
    A a;
    int i = 42;

    useTypeWithFunc(a, i); // 正确调用A::func(int)

    // useTypeWithFunc(a, "string"); // 编译错误，因为参数类型不是int
    return 0;
}
