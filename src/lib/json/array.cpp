#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/* parser json to argv format */
int main() {
  std::ifstream f("../array.json");
  std::vector<std::string> args;
  if(f.is_open()) {
    json data = json::parse(f);
    json eal = data["EAL_CONFIG"];
    if(!eal.is_array()) {
      std::cout << "not array" << std::endl;
      return 0;
    }
    int argc = eal.size();
    char **argv = new char*[argc];
    for(int i = 0; i < argc; i++) {
      std::string arg = eal[i].template get<std::string>();
      int size = arg.size();
      argv[i] = new char[size+1];
      strncpy(argv[i], arg.c_str(), size);
      argv[i][size] = '\0';
    }

    for(int i = 0; i < argc; i++) {
      std::cout << argv[i] << std::endl;
    }

    for(int i = 0; i < argc; i++) {
      delete argv[i];
    }
    delete []argv;
  }

  return 0;
}
