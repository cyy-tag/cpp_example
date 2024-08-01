#include <liburing.h>
#include <fcntl.h>    // open, O_WRONLY, O_CREAT
#include <unistd.h>   // close, write
#include <stdio.h>    // perror, printf
#include <stdlib.h>   // malloc, free
#include <string.h>   // strerror
#include <chrono>
#include <iostream>

#define NUM_WRITES 1000 // 写入操作的数量
#define DATA_SIZE 1024 // 每个写入操作的数据大小

int main() {
    const char *filename = "bulk_data.txt"; // 目标文件名

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
    if (io_uring_queue_init(1, &ring, 0) < 0) {
        perror("Failed to initialize io_uring queue");
        close(fd);
        for (int i = 0; i < NUM_WRITES; ++i) {
            free(data_blocks[i]);
        }
        free(data_blocks);
        return 1;
    }

    // 准备写入操作
    struct iovec *iovecs = (struct iovec *)malloc(NUM_WRITES * sizeof(struct iovec));
    for (int i = 0; i < NUM_WRITES; ++i) {
        iovecs[i].iov_base = data_blocks[i];
        iovecs[i].iov_len = DATA_SIZE;
    }

    // 获取 SQE 并准备写入操作
    auto now = std::chrono::steady_clock::now();
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, fd, iovecs, NUM_WRITES, 0); // 写入操作

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
    auto finish = std::chrono::steady_clock::now();
    auto cost = finish - now;
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(cost).count() << std::endl;
    // 等待 IO 完成
    struct io_uring_cqe *cqe;
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
