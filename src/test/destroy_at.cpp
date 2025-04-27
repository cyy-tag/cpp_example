#include <iostream>
#include <memory>
#include <new>

struct Tracer {
  Tracer(int i): value(i) {
    std::cout << "construct " << i << std::endl;
  }
  int value;
  ~Tracer() { std::cout << value << " destructed\n"; }
};

int main(int argc, char*argv[])
{
  alignas(Tracer) unsigned char buffer[sizeof(Tracer)*8];
  for(int i = 0; i < 8; ++i) {
    //通过reinterpret_cast<Tracer*> 转换为要构造的目标类型
    std::construct_at(reinterpret_cast<Tracer*>(buffer + sizeof(Tracer)*i), i);
  }

  auto ptr = std::launder(reinterpret_cast<Tracer*>(buffer));
  for(int i = 0; i < 8; ++i)
    std::destroy_at(ptr+i);
  return 0;
}
