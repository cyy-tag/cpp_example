#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include <asio.hpp>

void async_task(asio::io_context& io, int task_id) {
    asio::post(io, [task_id]() {
        // std::cout << "Task " << task_id << " running in thread " << std::this_thread::get_id() << std::endl;
        // 模拟一些工作
        // std::this_thread::sleep_for(asio::chrono::microseconds(1));
    });
}

int main(int argc, char* argv[]) {

    int thread_count = atoi(argv[1]); //std::thread::hardware_concurrency();  // 使用硬件支持的最大线程数
    const int task_count = 2097152;  // 每个线程提交的任务数量
    std::cout << "thread_count: " << thread_count << std::endl;
    asio::io_context io;
    asio::io_context::work work(io);
    std::vector<std::thread> threads;

    // 创建并启动线程
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&io]() {
            io.run();
        });
    }
    std::cout << "start commit: " << std::endl;
    auto start = std::chrono::steady_clock::now();
    // 提交任务
    for (int i = 0; i < task_count; ++i) {
        async_task(io, i);
    }
    auto finish = std::chrono::steady_clock::now();
    auto cost = finish - start;
    std::cout << "commit1 cost " << std::chrono::duration_cast<std::chrono::milliseconds>(cost).count() << std::endl;
    // 等待所有任务完成
    for (auto& t : threads) {
        t.join();
    }

    std::cout << "All tasks completed." << std::endl;

    return 0;
}
