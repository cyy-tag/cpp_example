#include <iostream>
#include <type_traits>

// 函数原型声明
int myFunction(int, double);

// 函数模板，用于推断函数的返回类型
template<typename Func, typename... Args>
struct function_traits {
    using return_type = decltype(std::declval<Func>()(std::declval<Args>()...));
};

// 实现有参数的普通函数
int myFunction(int x, double y) {
    return static_cast<int>(x + y);
}

int main() {
    // 使用 function_traits 来推断 myFunction 的返回类型
    using ReturnType = function_traits<decltype(myFunction), int, double>::return_type;
    std::cout << "The return type of myFunction is: " << typeid(ReturnType).name() << std::endl;
}
