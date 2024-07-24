#include <type_traits>

template<typename Func>
struct function_traits : public function_traits<decltype(&Func::operator())> {};

template<typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const> {
    using return_type = ReturnType;
};

// 示例函数
class MyFunctor {
public:
    int operator()(int x, double y) const {
        return static_cast<int>(x + y);
    }
};


int main() {
    using ReturnType = typename function_traits<MyFunctor>::return_type;
    static_assert(std::is_same<ReturnType, int>::value, "Return type must be int");

    MyFunctor functor;
    ReturnType result = functor(1, 2.5);
}
