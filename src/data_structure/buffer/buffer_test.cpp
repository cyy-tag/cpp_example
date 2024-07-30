#include <iostream>
#include <cstring>
#include "buffer.h"
#include <random>


int main()
{
  constexpr const int len = 10;
  constexpr const size_t size = len*sizeof(int);
  int data[len]{};
  for(int i = 0; i < len; ++i) {
    data[i] = rand();
  }
  Buffer buffer(3000);
  buffer.Append((const uint8_t*)data, size);
  buffer.Append((const uint8_t*)data, size);
  buffer.Append((const uint8_t*)data, size);
  int index = 0;
  for(int i = 0; i < 3; ++i) {
    if(memcmp(data, buffer.Data()+index, sizeof(data)-1)) {
      std::cout << "err" << std::endl;
    }
    index += size;
  }
}
