#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <chrono>
using namespace std;

struct Param {
  uint64_t block_size_;
  uint64_t batch_cnt_;
  uint64_t total_size_;
};

int main(int argc, char*argv[]) {
    if(argc < 3) {
        printf("usage: block_size batch_cnt total_size \n");
        return 0;
    }

    Param param;
    param.block_size_ = atoll(argv[1]);
    param.batch_cnt_ = atoll(argv[2]);
    param.total_size_ = atoll(argv[3]);
    printf("packet_size %ld batch_cnt %ld total_size %ld\n",
          param.batch_cnt_, param.batch_cnt_, param.total_size_);

    char * buffer = (char*)malloc(param.block_size_);
    memset(buffer, 'a', param.block_size_);

    //打开要写入的文件
    const char *filename = "bulk_data.txt"; // 目标文件名
    int fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) {
        printf("Failed to open file %s\n", strerror(errno));
        return -1;
    }

    //初始化io_uring
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    struct io_uring_params params;
    //初始化 io_uring_params 结构体
    memset(&params, 0, sizeof(params));
    // 设置 SQPOLL 标志，请求内核自动轮询提交队列
    params.flags |= IORING_SETUP_SQPOLL;

    // 初始化 io_uring 实例
    if (int ret = io_uring_queue_init_params(16384, &ring, &params); ret < 0) {
        printf("io_uring_queue_init %s\n", strerror(-ret));
    }

    // 检查是否成功设置了 SQPOLL
    if (!(params.features & IORING_SETUP_SQPOLL)) {
        fprintf(stderr, "SQPOLL is not supported.\n");
        io_uring_queue_exit(&ring);
        return EXIT_FAILURE;
    }

    //注册文件描述符到liburing中
    int fds[2];
    fds[0] = fd;
    int ret = io_uring_register_files(&ring, fds, 1);
    if(ret) {
        fprintf(stderr, "Error registering buffers: %s", strerror(-ret));
        return 1;
    }

    uint64_t cur_size = 0;
    int times = 0;
    // 准备写入操作
    struct iovec *iovecs = (struct iovec *)malloc(param.batch_cnt_ * sizeof(struct iovec));
    while(cur_size < param.total_size_) {
      //每次批量操作
      for(int i = 0; i < param.batch_cnt_; ++i)
      {
        iovecs[i].iov_base = buffer;
        iovecs[i].iov_len = param.block_size_;
      }
      sqe = io_uring_get_sqe(&ring);
      if(!sqe) {
        printf("no io seq exits\n");
        return -1;
      }
      io_uring_prep_writev(sqe, 0, iovecs, param.batch_cnt_, 0);
      sqe->flags |= IOSQE_FIXED_FILE;
      if(int ret = io_uring_submit(&ring); ret < 0) {
        printf("io_uring submit error %s\n", strerror(-ret));
        return 0;
      }
      cur_size += param.block_size_ * param.batch_cnt_;
      ++times;
    }

    printf("submit sqe count %d\n", times);
    for(int cnt = 0; cnt < times; ++cnt) {
      int ret = io_uring_wait_cqe(&ring, &cqe);
      if(ret < 0) {
        if(errno == EINTR) {
          continue;
        }
        printf("io_uring_wait_cqe error %s\n", strerror(-ret));
        return -1;
      }

      if(cqe->res < 0) {
        printf("Error in async operation: %s\n", strerror(-cqe->res));
        return -1;
      }
      // 通知内核 CQE 已经被处理
      io_uring_cqe_seen(&ring, cqe);
    }

    printf("complete\n");
    // 清理 io_uring 实例
    io_uring_queue_exit(&ring);

    return EXIT_SUCCESS;
}
