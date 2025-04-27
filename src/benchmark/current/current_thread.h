#pragma once
#include <unistd.h>
#include <sys/syscall.h>

namespace storage {


inline int CurrentTid()
{
  thread_local int tid = static_cast<pid_t>(::syscall(SYS_gettid));
  return tid;
}

} // namespace storage
