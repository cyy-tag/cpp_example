#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/* parser json to argv format */
int main() {
  std::ifstream f("../config/object.json");
  if(f.is_open()) {
    json data = json::parse(f);
    auto socket_buf = data["socket_buffer"];
    std::cout << socket_buf.is_object() << std::endl;
    //遍历操作
    for(auto& x : socket_buf.items()) {
      //key string 转 int 失败报错
      int socket_id = stoi(x.key());
      int buffer_size = x.value();
      std::cout << "key: " << socket_id << ", value: " << buffer_size << std::endl;
    }

    //int
    uint16_t  val = data["test_int"].template get<uint16_t >();
    std::cout << val << std::endl;
  } else {
    std::cout << "can't open object.json" << std::endl;
  }
  return 0;
}
