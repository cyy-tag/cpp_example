#include <ctime>
#include <iostream>

using namespace std;

time_t convert_str_to_timestamp(const string& str) {
  tm tm_time;
  // 假设字符串格式为 "YYYY-MM-DD HH:MM:SS"
  if (sscanf(str.c_str(), "%d-%d-%d %d:%d:%d", &tm_time.tm_year, &tm_time.tm_mon, &tm_time.tm_mday,
              &tm_time.tm_hour, &tm_time.tm_min, &tm_time.tm_sec) == 6) {
      tm_time.tm_year -= 1900;
      tm_time.tm_mon -= 1;
      tm_time.tm_isdst = 0;
      return mktime(&tm_time);
  }
  cout << tm_time.tm_year << " " << tm_time.tm_mon << " " << tm_time.tm_mday << endl;
  return -1; // 如果转换失败，返回-1
}

int main() {
  string str_time = "2023-03-25 15:30:00"; // 示例时间字符串
  time_t timestamp = convert_str_to_timestamp(str_time);
  if (timestamp != -1) {
      cout << "时间戳为: " << timestamp << endl;
  } else {
      cout << "字符串转换为时间戳失败" << endl;
  }
  return 0;
}
