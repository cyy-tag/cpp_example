#include <liburing.h>
#include <fcntl.h>    // open, O_WRONLY, O_CREAT, O_TRUNC
#include <unistd.h>   // close, write
#include <stdio.h>    // perror, printf
#include <stdlib.h>   // exit, malloc, free
#include <string.h>   // strerror

int main() {
    const char *filename = "example.txt"; // 要写入的文件名
    const char *data = "Hello, World! This is a test of async write."; // 要写入的数据

    // 打开文件用于写入
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open file");
        return 1;
    }

    // 初始化 io_uring 实例
    struct io_uring ring;
    if (io_uring_queue_init(1, &ring, 0) < 0) { // 1 是请求队列的大小
        perror("Failed to initialize io_uring queue");
        close(fd);
        return 1;
    }

    // 获取一个 SQE 并准备写入操作
    struct io_uring_sqe *sqe = io_uring_get_sqe(&ring);
    io_uring_prep_write(sqe, fd, data, strlen(data), 0); // 异步写入

    // 提交 SQE 给内核
    if (io_uring_submit(&ring) < 0) {
        perror("Failed to submit IO request");
        io_uring_queue_exit(&ring);
        close(fd);
        return 1;
    }

    // 等待 IO 完成
    struct io_uring_cqe *cqe;
    if (io_uring_wait_cqe(&ring, &cqe)) {
        perror("Failed to wait for IO completion");
        io_uring_queue_exit(&ring);
        close(fd);
        return 1;
    }

    // 检查写入结果
    if (cqe->res < 0) {
        fprintf(stderr, "IO error: %s\n", strerror(-cqe->res));
    } else {
        printf("Write successful, bytes written: %d\n", cqe->res);
    }

    // 清理资源
    io_uring_cqe_seen(&ring, cqe); // 标记 CQE 已经被处理
    io_uring_queue_exit(&ring);
    close(fd);

    return 0;
}
