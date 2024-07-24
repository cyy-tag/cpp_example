#pragma once
#include <sys/time.h>
#include <chrono>

inline int64_t GetMicroTime() {
	struct timeval value;
	gettimeofday(&value, nullptr);
	return (int64_t)(value.tv_sec) * 1000000LL + (int64_t)(value.tv_usec);
}

inline int64_t GetCurretTime() {
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}
