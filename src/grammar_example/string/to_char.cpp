#include <array>
#include <vector>
#include <charconv>
#include <iostream>
#include <string_view>
#include <system_error>

//std::array在栈上创建,不能使用太大
// struct TestArray {
//     // std::array<char, 8024> str;
// }
 
void show_to_chars(auto... format_args)
{
    std::vector<char> str(10);
 
#if __cpp_lib_to_chars >= 202306L and __cpp_structured_bindins >= 202406L
    // use C++26 structured bindins declaration as condition (P0963)
    // and C++26 to_chars_result::operator bool() for error checking (P2497)
    if (auto [ptr, ec] =
            std::to_chars(str.data(), str.data() + str.size(), format_args...))
        std::cout << std::string_view(str.data(), ptr) << '\n';
    else
        std::cout << std::make_error_code(ec).message() << '\n';
#elif __cpp_lib_to_chars >= 202306L
    // use C++26 to_chars_result::operator bool() for error checking (P2497)
    if (auto result =
            std::to_chars(str.data(), str.data() + str.size(), format_args...))
        std::cout << std::string_view(str.data(), result.ptr) << '\n';
    else
        std::cout << std::make_error_code(result.ec).message() << '\n';
#else
    // fallback to C++17 if-with-initializer and structured bindings
    if (auto [ptr, ec] =
            std::to_chars(str.data(), str.data() + str.size(), format_args...);
        ec == std::errc())
        std::cout << std::string_view(str.data(), ptr) << '\n';
    else
        std::cout << std::make_error_code(ec).message() << '\n';
#endif
}

void test(uint8_t mac[6]) {
    std::cout << 1111 << std::endl;
}
 
int main()
{
    // show_to_chars(42);
    // show_to_chars(+3.14159F);
    // show_to_chars(-3.14159, std::chars_format::fixed);
    // show_to_chars(-3.14159, std::chars_format::scientific, 3);
    // show_to_chars(3.1415926535, std::chars_format::fixed, 10);
    // std::array<char, 20> str;
    // std::vector<char> str(20);
    // uint64_t val = 1111111111111111;
    // auto [ptr, ec] = std::to_chars(str.data(), str.data()+str.size(), val);
    // if(ec == std::errc()) {
    //     std::cout << 222222222 << std::endl;
    // }
    // std::cout << std::make_error_code(ec).message() << std::endl;
    // std::cout << std::string_view(str.data(), ptr) << '\n';
    return 0;
}
