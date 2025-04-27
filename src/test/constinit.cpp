#include "constinit.h"

thread_local std::array<Foo, Test::max_mod_> Test::num{};

constexpr Foo::Foo()
 :a(4)
{}

int main()
{
    Test test;
    for(auto & f : Test::num) {
        std::cout << f.a << std::endl;
    }
}
