#include <iostream>
#include <concepts>

// Syntax
template <class Expr>
consteval bool is_consteval(Expr)
    { return requires { typename std::bool_constant<(Expr{}(), true)>; }; }


int main(int argc, char*argv[])
{
  int a = 10;
  if(is_consteval([]{ return false; })) {
    std::cout << "const" << std::endl;
  } else {
    std::cout << "no const" << std::endl;
  }
  return 0;
}
