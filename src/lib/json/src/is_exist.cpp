#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


int main() {
  std::ifstream f("../config/is_exist.json");
  if(f.is_open()) {
    json data = json::parse(f);
    if(data.find("key") != data.end()) {
      std::cout << "key exist" << std::endl;
    } else {
      std::cout << "key not exist" << std::endl;
    }
  } else {
    std::cout << " cannot open file " << std::endl;
  }
  return 0;
}
