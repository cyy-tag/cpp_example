#pragma once

class Buffer
{
public:
  /// Construct an empty buffer.
  Buffer() noexcept
    : data_(0),
      size_(0)
  {
  }

  /// Construct a buffer to represent a given memory range.
  Buffer(void* data, std::size_t size) noexcept
    : data_(data),
      size_(size)
  {
  }

  inline void Reset(void * data, std::size_t size) noexcept
  {
    data_ = data;
    size_ = size;
  }

  const void* data() const noexcept
  {
    return data_;
  }

  /// Get the size of the memory range.
  std::size_t size() const noexcept
  {
    return size_;
  }

  /// Move the start of the buffer by the specified number of bytes.
  Buffer& operator+=(std::size_t n) noexcept
  {
    std::size_t offset = n < size_ ? n : size_;
    data_ = data_ + offset;
    size_ -= offset;
    return *this;
  }

private:
  void* data_;
  std::size_t size_;
};
