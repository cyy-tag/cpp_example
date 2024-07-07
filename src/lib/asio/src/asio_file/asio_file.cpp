#include <iostream>
#include <thread>
#include <chrono>
#include "file.h"

int main()
{
  std::cout << "hello world" << std::endl;
  const int block_size = 1024;
  const int block_cnt = 1024*1024/8;
  const int gather_cnt = 16; // pow(2, n);
  char data[block_size];
  File file(FileEventHandlers{}, "../", 1000);
  uint64_t write_size = 0;
  auto now = std::chrono::steady_clock::now();
  for(int i = 0; i < block_cnt/gather_cnt; i++)
  {
    std::vector<asio::const_buffer> buffers;
    for(int j = 0; j < gather_cnt; j++) {
      buffers.push_back(asio::buffer(data, block_size));
    }
    file.AsyncWrite(buffers, [&](){
      write_size += block_size*gather_cnt;
    });
  }
  auto cost_write = std::chrono::steady_clock::now() - now;
  std::cout << "cost_write: " << std::chrono::duration_cast<std::chrono::milliseconds>(cost_write) << std::endl;
  std::cout << " commit complete" << std::endl;
  while(true){
    file.RunAsyncLoop();
    if(write_size == block_cnt * block_size) {
      auto cost = std::chrono::steady_clock::now() - now;
      std::cout << "write_complete: " << std::chrono::duration_cast<std::chrono::milliseconds>(cost) << std::endl;
      break;
    }
  }
  return 0;
}