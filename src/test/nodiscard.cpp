#include <iostream>

using namespace std;

//返回值不能忽略
[[nodiscard]]
int boo() {
  return 1;
}

int main(int argc, char*argv[])
{
  //需要加上变量未使用告警限制
  int val = boo();
  cout << val << endl;
  // boo(); 没有处理返回值编译器告警
  return val;
}

