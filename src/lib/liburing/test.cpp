#include <liburing.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <chrono>
using namespace std;

// 简单的错误处理函数
#define ERR_EXIT(ptr, msg) \
    do { fprintf(stderr, "%s: %s\n", msg, strerror(errno)); exit(EXIT_FAILURE); } while (0)

int main(int argc, char*argv[]) {
    const char *filename = "bulk_data.txt"; // 目标文件名
    if(argc < 3) {
        printf("usage packet_size  batch total_size\n");
        return 0;
    }

    int NUM_WRITES = atoi(argv[1]);
    int DATA_SIZE = atoi(argv[2]);
    char **data_blocks = (char **)malloc(NUM_WRITES * sizeof(char *));
    for (int i = 0; i < NUM_WRITES; ++i) {
        data_blocks[i] = (char *)malloc(DATA_SIZE);
        memset(data_blocks[i], 'A' + i, DATA_SIZE); // 初始化数据
    }
    struct io_uring ring;
    struct io_uring_sqe *sqe;
    struct io_uring_cqe *cqe;
    struct io_uring_params params;
    const char *buffer = "Hello, World!";
    size_t buffer_size = strlen((const char *)buffer) + 1;
    // 打开文件用于写入
    int fd = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        for (int i = 0; i < NUM_WRITES; ++i) {
            free(data_blocks[i]);
        }
        free(data_blocks);
        return 1;
    }

    // 初始化 io_uring_params 结构体
    memset(&params, 0, sizeof(params));
    // 设置 SQPOLL 标志，请求内核自动轮询提交队列
    params.flags |= IORING_SETUP_SQPOLL;

    // 初始化 io_uring 实例
    if (io_uring_queue_init_params(32, &ring, &params) < 0) {
        ERR_EXIT(NULL, "io_uring_queue_init");
    }

    // 检查是否成功设置了 SQPOLL
    if (!(params.features & IORING_SETUP_SQPOLL)) {
        fprintf(stderr, "SQPOLL is not supported.\n");
        io_uring_queue_exit(&ring);
        return EXIT_FAILURE;
    }
    int fds[2];
    fds[0] = fd;
    int ret = io_uring_register_files(&ring, fds, 1);
    if(ret) {
        fprintf(stderr, "Error registering buffers: %s", strerror(-ret));
        return 1;
    }
    // 准备一个写入操作
    sqe = io_uring_get_sqe(&ring);
    if (!sqe) {
        ERR_EXIT(NULL, "io_uring_get_sqe");
    }

    // 准备写入操作
    struct iovec *iovecs = (struct iovec *)malloc(NUM_WRITES * sizeof(struct iovec));
    auto now = chrono::steady_clock::now();
    for (int i = 0; i < NUM_WRITES; ++i) {
        iovecs[i].iov_base = data_blocks[i];
        iovecs[i].iov_len = DATA_SIZE;
    }

    io_uring_prep_writev(sqe, 0, iovecs, NUM_WRITES, 0);
    sqe->flags |= IOSQE_FIXED_FILE;

    // 提交 I/O 请求
    if (io_uring_submit(&ring) < 0) {
        ERR_EXIT(NULL, "io_uring_submit");
    }

    // 轮询完成队列以等待写入操作完成
    while (1) {
        int ret = io_uring_wait_cqe(&ring, &cqe);
        if (ret < 0) {
            if (errno == EINTR) {
                // 被信号中断，可以继续等待
                continue;
            }
            ERR_EXIT(NULL, "io_uring_wait_cqe");
        }
        break;
    }
    // 检查 I/O 操作结果
    if (cqe->res < 0) {
        fprintf(stderr, "Error in async operation: %s\n", strerror(-cqe->res));
        return 0;
    }

    printf("Wrote %d bytes\n", cqe->res);

    // 通知内核 CQE 已经被处理
    io_uring_cqe_seen(&ring, cqe);

    // 清理 io_uring 实例
    io_uring_queue_exit(&ring);

    return EXIT_SUCCESS;
}