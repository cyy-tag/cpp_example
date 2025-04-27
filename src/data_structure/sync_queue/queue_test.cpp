#include <thread>
#include <iostream>
#include "sync_queue.h"

class File 
{
public:
  File()=default;
};

int main()
{

  SyncQueue<File*> queue;
  File* file=nullptr;
  File* empty = new File();
  queue.Enqueue(empty);
  if(queue.DequeueWithTimeout(file, std::chrono::seconds(1))) {
    std::cout << (file == empty) << std::endl;
    std::cout << "get file" << std::endl;
  } else {
    std::cout << "time out " << std::endl;
  }

}