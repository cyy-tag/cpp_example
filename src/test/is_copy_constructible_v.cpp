#include <type_traits>
#include <iostream>

struct MyStruct {
  int value;
};

struct NonCopyable {
  NonCopyable() = default;
  NonCopyable(const NonCopyable&) = delete;
};

int main(int argc, char *argv[])
{
  std::cout << "MyStruct is copy constructible: "
  << std::is_copy_constructible_v<MyStruct> << std::endl;
  std::cout << "NonCopyable is copy constructible: "
  << std::is_copy_constructible_v<NonCopyable> << std::endl;
  return 0;
}