#include <iostream>
#include <thread>
#include <chrono>
#include "file.h"

int main(int argc, char*argv[])
{
  if(argc < 4) {
    std::cout << "usage asio_file block_size block_cnt batch_cnt" << std::endl;
  }
  long long block_size = atoll(argv[1]);
  long long block_cnt = atoll(argv[2]);
  long long batch_cnt = atoll(argv[3]);

  long long total_size = block_size*block_cnt;
  long long write_size = 0;
  std::vector<char> data(block_size, 0);
  std::vector<std::vector<asio::const_buffer>> buf;
  if(batch_cnt > 0) {
    buf.resize(block_cnt/batch_cnt);
  }
  File file(FileEventHandlers{}, "../", 1000);
  auto now = std::chrono::steady_clock::now();
  if(batch_cnt == 0) {
    file.AsyncWrite((uint8_t *)data.data(), data.size(), [&]
    ()
    mutable
    {
        write_size += block_size;
    });
  } else {
    for(int i = 0; i < block_cnt/batch_cnt; i++)
    {
      auto &buffers = buf[i];
      for(int j = 0; j < batch_cnt; j++) {
        buffers.push_back(asio::buffer(data, block_size));
      }
      file.AsyncWrite(buffers, [&](){
        write_size += block_size*batch_cnt;
      });
    }
  }
  auto cost_write = std::chrono::steady_clock::now() - now;
  std::cout << "cost_write: " << std::chrono::duration_cast<std::chrono::microseconds>(cost_write).count() << std::endl;
  std::cout << " commit complete" << std::endl;
  while(true){
    file.RunAsyncLoop();
    if(write_size == block_cnt * block_size) {
      auto cost = std::chrono::steady_clock::now() - now;
      std::cout << "write_complete: " << std::chrono::duration_cast<std::chrono::milliseconds>(cost).count() << std::endl;
      break;
    }
  }
  return 0;
}
