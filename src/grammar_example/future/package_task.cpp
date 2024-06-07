#include <future>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

int main()
{
  std::packaged_task<int()> task([]{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 1;
  });

  std::future<int> f = task.get_future();
  std::thread t(std::move(task));

  std::cout << f.get() << std::endl;
  t.join();
  return 0;
}
