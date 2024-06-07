#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    std::string path = "/mnt/g/rttwc/filter_data/"; // 替换为你的目录路径

    int start = 9;
    int end = 10;
    try {
        if (fs::exists(path) && fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (fs::is_regular_file(entry.status())) {
                    std::string file_name = entry.path().filename();
                    if(file_name.size() != 17) {
                      continue;
                    }
                    int hour = (file_name[9] - '0') * 10 + (file_name[10] - '0');
                    if(hour < start || hour > end) {
                      continue;
                    }
                    std::cout << file_name << std::endl;
                }
            }
        } else {
            std::cerr << "Path does not exist or is not a directory." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << e.what() << std::endl;
    }
 
    return 0;
}
