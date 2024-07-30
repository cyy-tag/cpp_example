#pragma once
#include <cassert>
#include <vector>

class Buffer
{
 public:
  explicit Buffer(size_t size)
    : buffer_(size), write_index_(0)
  {
    assert(size > 0);
  }

  void Append(const uint8_t *data, size_t size)
  {
    assert(write_index_+size < buffer_.size());
    memcpy(&(buffer_[0])+write_index_, data, size);
    write_index_ += size;
  }

  size_t WritableBytes() {
    return buffer_.size() - write_index_;
  }

  size_t Size() {
    return write_index_;
  }

  const void* Data() {
    return &(buffer_[0]);
  }

  void Reset() {
    write_index_ = 0;
  }

private:
  std::vector<char> buffer_;
  size_t write_index_;
};