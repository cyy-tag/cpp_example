#pragma once
#include <mutex>
#include <condition_variable>
#include <queue>

template<typename T>
class SyncQueue {
public:
  void Enqueue(T value) {
    std::lock_guard lock(mutex_);
    queue_.push(std::move(value));
    cond_var_.notify_one();
  }

  bool TryDequeue(T& value) {
    std::lock_guard lock(mutex_);
    if(queue_.empty()) {
      return false;
    }
    value = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  void Dequeue(T& value) {
    std::unique_lock lock(mutex_);
    cond_var_.wait(lock, [this]{ return !queue_.empty(); });
    value = std::move(queue_.front());
    queue_.pop();
  }

private:
  std::queue<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_var_;
};
