#include <ranges>
#include "file.h"
#include <iostream>

File::File(FileEventHandlers event_handlers, fs::path dir, size_t file_size)
 :dir_(dir), file_size_(file_size), event_handlers_(std::move(event_handlers)),
  file_(io_context_)
{
  Open("./test.pcap");
}

void File::Open(const std::string & fname)
{
  Close();
  cur_file_name_ = fname;
  std::error_code ec;
  file_.open(fname,
          asio::stream_file::truncate
            | asio::stream_file::create
            | asio::stream_file::write_only,
          ec);
  if(ec) {
    std::cerr << "file open err filename "
              << cur_file_name_ << " err_msg "
              << ec.message() << std::endl;
  }
  if(event_handlers_.after_open)
  {
    event_handlers_.after_open(cur_file_name_, ec);
  }
}

void File::RunAsyncLoop()
{
  io_context_.run();
}

void File::AsyncWrite(const uint8_t *data, size_t size, WriteCallBack write_callback)
{
  asio::async_write(file_, asio::buffer(data, size),
    [write_callback](std::error_code error, std::size_t n)
    {
      if(error){
        std::cerr << "error write message " << error.message() << std::endl;
      }
      write_callback();
    });
}

void File::AsyncWrite(std::vector<asio::const_buffer> &buffers, WriteCallBack write_callback)
{
  asio::async_write(file_, std::views::all(buffers),
  [write_callback](std::error_code error, std::size_t n)
  {
    if(error){
      std::cerr << "error write message " << error.message() << std::endl;
    }
    write_callback();
  });
}

void File::Sync()
{
  if(file_.is_open()) {
    asio::error_code ec;
    file_.sync_all(ec);
    if(ec){
      std::cerr << "file sync err filename "
                << cur_file_name_ << " err_msg "
                << ec.message() << std::endl;
    }
  }
}

void File::Close()
{
  if(file_.is_open()) {
    asio::error_code ec;
    file_.close(ec);
    if(ec) {
      std::cerr << "file close err filename "
                << cur_file_name_ << " err_msg "
                << ec.message() << std::endl;
    }
    if(event_handlers_.after_close) {
      event_handlers_.after_close(cur_file_name_, file_size_, ec);
    }
  }
  file_size_ = 0;
}
