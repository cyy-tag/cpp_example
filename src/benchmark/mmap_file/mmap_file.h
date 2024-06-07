#pragma once
#include <sys/types.h>
#include <unistd.h> //close
#include <sys/stat.h>
#include <sys/mman.h> //mmap函数的必要头文件
#include <fcntl.h> // struct stat 需要的头文件

int mmap_file(const char * path, void ** buf, size_t * buf_size) {
  int fd = -1;
  if ( (fd = open(path, O_RDWR)) < 0) {
    return -1;
  }
  
  struct stat st;
  if(fstat(fd, &st) == -1) {
    close(fd);
    return -1;
  }

  if((*buf = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE|MAP_POPULATE, fd, 0)) == (void*)-1) {
    close(fd);
    return -1;
  }
  *buf_size = st.st_size;
  return 0;
}
