#include <sys/eventfd.h>
#include <cstdint>
#include <cstdio>
#include <unistd.h>

int main()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  uint64_t one = 1;
  ssize_t n  = 0;
  for(uint64_t i = 1; i < 64; ++i) {
    ++one;
    ssize_t n = ::write(evtfd, &one, sizeof one);
    if (n != sizeof one) {
      printf(" write %ld\n", n);
      return -1;
    }
  }

  one = 1;
  n = ::read(evtfd, &one, sizeof one);
  printf("read %ld one %ld\n", n, one);
  return 0;
}