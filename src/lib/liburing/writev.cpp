#include <liburing.h>
#include <fcntl.h>    // open, O_WRONLY, O_CREAT
#include <unistd.h>   // close, write
#include <stdio.h>    // perror, printf
#include <stdlib.h>   // malloc, free
#include <string.h>   // strerror
#include <chrono>
#include <iostream>

using namespace std;
// #define NUM_WRITES 16777216 // 写入操作的数量
// #define DATA_SIZE 64 // 每个写入操作的数据大小

int main(int argc, char *argv[]) {
    const char *filename = "bulk_data.txt"; // 目标文件名
    if(argc < 3) {
        printf("usage packet_size  batch total_size\n");
        return 0;
    }

    int NUM_WRITES = atoi(argv[1]);
    int DATA_SIZE = atoi(argv[2]);

    // 创建一个包含多个数据块的数组
    char **data_blocks = (char **)malloc(NUM_WRITES * sizeof(char *));
    for (int i = 0; i < NUM_WRITES; ++i) {
        data_blocks[i] = (char *)malloc(DATA_SIZE);
        memset(data_blocks[i], 'A' + i, DATA_SIZE); // 初始化数据
    }

    // 打开文件用于写入
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        for (int i = 0; i < NUM_WRITES; ++i) {
            free(data_blocks[i]);
        }
        free(data_blocks);
        return 1;
    }

    // 初始化 io_uring 实例
    struct io_uring ring;
    struct io_uring_params params;
    // 初始化 io_uring_params 结构体
    memset(&params, 0, sizeof(params));
    // 设置 SQPOLL 标志
    // params.flags = IORING_SETUP_SQPOLL;
    // int ret = io_uring_setup(64, &params);
    // if(ret < 0) {
    //     printf("111 Failed to initialize io_uring queue %s\n", strerror(-ret));
    //     return 0;
    // }
    if (int ret =  io_uring_queue_init(4096, &ring, IORING_SETUP_SQPOLL); ret < 0) {
        printf("Failed to initialize io_uring queue %s\n", strerror(-ret));
        close(fd);
        for (int i = 0; i < NUM_WRITES; ++i) {
            free(data_blocks[i]);
        }
        free(data_blocks);
        return 1;
    }

    if(IO_URING_READ_ONCE(*ring.sq.kflags) & IORING_SQ_NEED_WAKEUP) {
        *ring.sq.kflags |= IORING_ENTER_SQ_WAKEUP;
    }
    // 准备写入操作
    struct iovec *iovecs = (struct iovec *)malloc(NUM_WRITES * sizeof(struct iovec));
    auto now = chrono::steady_clock::now();
    for (int i = 0; i < NUM_WRITES; ++i) {
        iovecs[i].iov_base = data_blocks[i];
        iovecs[i].iov_len = DATA_SIZE;
    }
    auto finish = chrono::steady_clock::now();
    auto cost = finish - now;
    std::cout << " prepare buffers " << chrono::duration_cast<chrono::microseconds>(cost).count() << std::endl;
    // 获取 SQE 并准备写入操作
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    now = chrono::steady_clock::now();
    io_uring_prep_writev(sqe, fd, iovecs, NUM_WRITES, 0); // 写入操作
    finish = chrono::steady_clock::now();
    cost = finish - now;
    std::cout << "io_uring_prep_writev " << chrono::duration_cast<chrono::microseconds>(cost).count() << std::endl;
    now = std::chrono::steady_clock::now();
    // 提交 SQEs 给内核
    if (io_uring_submit(&ring) < 0) {
        perror("Failed to submit IO request");
        io_uring_queue_exit(&ring);
        close(fd);
        free(iovecs);
        for (int i = 0; i < NUM_WRITES; ++i) {
            free(data_blocks[i]);
        }
        free(data_blocks);
        return 1;
    }
    finish = std::chrono::steady_clock::now();
    cost = finish - now;
    std::cout << "submit " << std::chrono::duration_cast<std::chrono::microseconds>(cost).count() << std::endl;
    // 等待 IO 完成
    struct io_uring_cqe *cqe;
    now = std::chrono::steady_clock::now();
    if (io_uring_wait_cqe(&ring, &cqe) < 0) {
        perror("Failed to wait for IO completion");
    } else {
        if (cqe->res < 0) {
            fprintf(stderr, "Write error: %s\n", strerror(-cqe->res));
        } else {
            printf("Write successful, bytes written: %d\n", cqe->res);
        }
        io_uring_cqe_seen(&ring, cqe);
    }
    finish = std::chrono::steady_clock::now();
    cost = finish - now;
    std::cout << "complete " << std::chrono::duration_cast<std::chrono::microseconds>(cost).count() << std::endl;
    // 清理资源
    free(iovecs);
    for (int i = 0; i < NUM_WRITES; ++i) {
        free(data_blocks[i]);
    }
    free(data_blocks);
    io_uring_queue_exit(&ring);
    close(fd);

    return 0;
}
