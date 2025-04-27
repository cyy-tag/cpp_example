#include <liburing.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    struct io_uring ring;
    struct io_uring_cqe *cqe;
    struct io_uring_sqe *sqe;
    const char *filename = "example.txt";
    int fd, ret;

    // 初始化 io_uring 实例
    ret = io_uring_queue_init(8, &ring, 0);
    if (ret < 0) {
        fprintf(stderr, "io_uring_queue_init failed\n");
        return 1;
    }

    // 打开文件
    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    // 获取提交队列条目
    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, fd, buffer, sizeof(buffer), 0);

    // 提交 I/O 请求
    io_uring_submit(&ring);

    // 轮询完成队列
    while ((ret = io_uring_peek_batch_cqe(&ring, &cqe)) == 0) {
        // 没有完成的事件，可以在这里做其他事情
        usleep(1000);
    }

    if (ret < 0) {
        fprintf(stderr, "io_uring_peek_batch_cqe failed\n");
        return 1;
    }

    // 处理完成的事件
    if (cqe->res < 0) {
        fprintf(stderr, "read failed: %d\n", cqe->res);
    } else {
        printf("Read %d bytes\n", cqe->res);
    }

    // 清理资源
    close(fd);
    io_uring_queue_exit(&ring);

    return 0;
}
