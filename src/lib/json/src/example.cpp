#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// ...
int main() {
  std::ifstream f("../config/example.json");
  if(f.is_open()) {
    json data = json::parse(f);
    json cap_cfg = data["capture"];
    if(cap_cfg.is_array()) {
      for(const auto& item : cap_cfg) {
        std::string type = item["type"].template get<std::string>();
        std::cout << type << std::endl;
      }
    }
  }
  return 0;
}
