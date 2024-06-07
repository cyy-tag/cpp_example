#include "singleton.h"
#include <iostream>

using namespace std;

class TestSignal
{
public:
  void test(){}
private:
  int v_;
};

int main() {
  TestSignal &instance0 = Singleton<TestSignal>::instance();
  TestSignal &instance1 = Singleton<TestSignal>::instance();

  if(&instance0 != &instance1) {
    cout << "not equal" << endl;
  }
  return 0;
}
