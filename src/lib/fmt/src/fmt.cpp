#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <iostream>

#include <fmt/core.h> //format string
#include <fmt/ranges.h> //print container

int main() {
  //format string
  int value = 10;
  fmt::print("Hello, world! {}\n", value);
  std::string str = fmt::format("value={}\n", value);
  std::cout << str << std::endl;

  //print container
  std::vector<int> v{1, 2, 34};
  fmt::print("print vector={}\n", v);
  std::unordered_map<int,int> mp{{1, 2}, {3, 4}, {4, 6}};
  fmt::print("print map={}\n", mp);
  std::list<int> l;
  l.push_back(1);
  l.push_front(2);
  fmt::print("print list={}\n", l);
  return 0;
}
