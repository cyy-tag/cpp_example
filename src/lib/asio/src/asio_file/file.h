#pragma once
#include <filesystem>
#include <memory>
#include <stdio.h>
#include <string>
#include <functional>

#include <asio.hpp>

namespace fs = std::filesystem;
using WriteCallBack = std::function<void()>;

struct FileEventHandlers
{
    FileEventHandlers()
        : before_open(nullptr),
          after_open(nullptr),
          before_write(nullptr),
          after_close(nullptr)
    {}

    std::function<void(const std::string &filename)> before_open;
    std::function<void(const std::string &filename, const std::error_code & ec)> after_open;
    std::function<void(const std::string &filename, time_t ts, size_t size)> before_write;
    std::function<void(const std::string &filename, size_t size, const std::error_code & ec)> after_close;
};

class File
{
public:
    File(FileEventHandlers event_handlers, fs::path dir, size_t file_size);
    void Open(const std::string & fname);
    void AsyncWrite(const uint8_t *data, size_t size, WriteCallBack write_callback);
    void AsyncWrite(std::vector<asio::const_buffer> &buffers, WriteCallBack write_callback);
    void RunAsyncLoop();
    void Sync();
    void Close();
private:
    fs::path dir_;
    size_t file_size_;
    std::string cur_file_name_;
    FileEventHandlers event_handlers_;
    asio::io_context io_context_;
    asio::stream_file file_;
};
