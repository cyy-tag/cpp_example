#include "stacknontype.hpp"
#include <iostream>
#include <string>

int main()
{
  Stack<int,20> int20_stack;
  Stack<int,40> int40_stack;
  Stack<std::string, 40> string_stack;

  // manipulate stack of up to 20 ints
  int20_stack.Push(7);
  std::cout << int20_stack.Top() << '\n';
  string_stack.Pop();
  return -1;
}
