#include "mmap_file.h"
#include "net.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <chrono>
/*
读取tcpdump的网络包文件，统计每个ip节点的报文数和流量总大小输出
读取文件采用read和mmap方式进行比较
*/

struct ip_data
{
  std::unordered_map<int,int> ip_packet_size_;
  std::unordered_map<int,int> ip_packet_cnt_;
};

ip_data ipd;

void use_mmap(const char * filename) {
  char * buf = nullptr;
  size_t buf_size = 0;
  if(mmap_file(filename, (void **)&buf, &buf_size) != 0) {
    std::cout << " mmap file error " << std::endl;
    return;
  }
  int cur = 0;
  while(cur < buf_size) {
    erf_record_t* pkt_hdr = (erf_record_t*)(buf+cur);
    uint16_t wlen = ntohs(pkt_hdr->wlen);
    uint16_t caplen = ntohs(pkt_hdr->rlen);
    auto [protocol, err] = decode(reinterpret_cast<uint8_t*>(&pkt_hdr->rec)+2, wlen);
    cur += caplen;
    if(err != KDecodeSuccess) {
      continue;
    }
    ipd.ip_packet_size_[protocol.ipheader->src_ip] += wlen;
    ipd.ip_packet_size_[protocol.ipheader->dst_ip] += wlen;
    ipd.ip_packet_cnt_[protocol.ipheader->src_ip]++;
    ipd.ip_packet_cnt_[protocol.ipheader->dst_ip]++;
  }
}

void use_read(const char * filename) {
  std::FILE* fd = nullptr;
  uint8_t buffer[2048];
  if (( fd = fopen(filename, "rb")) == nullptr) {
    return;
  }
  size_t rt = fread(buffer, sizeof(erf_record_t), 1, fd);
  while(rt == 1) {
    erf_record_t* pkt_hdr = (erf_record_t*)buffer;
    uint16_t wlen = ntohs(pkt_hdr->wlen);
    uint16_t reclen = ntohs(pkt_hdr->rlen) - sizeof(erf_record_t);
    uint8_t* pkt_data = buffer + sizeof(erf_record_t);
    rt = fread(pkt_data, reclen, 1, fd);
    if(rt != 1) {
      break;
    }
    auto [protocol, err] = decode(reinterpret_cast<uint8_t*>(&pkt_hdr->rec)+2, wlen);
    if(err == KDecodeSuccess) {
      ipd.ip_packet_size_[protocol.ipheader->src_ip] += wlen;
      ipd.ip_packet_size_[protocol.ipheader->dst_ip] += wlen;
      ipd.ip_packet_cnt_[protocol.ipheader->src_ip]++;
      ipd.ip_packet_cnt_[protocol.ipheader->dst_ip]++;
    }
    //读取下一个包内容
    rt = fread(buffer, sizeof(erf_record_t), 1, fd);
  }
}

int main(int argc, char *argv[])
{
  if( argc < 2) {
    std::cout << " input test erf file" << std::endl;
    return 0;
  } 
  const char* filename = argv[1];
  auto before_time = std::chrono::steady_clock::now();
  use_mmap(filename);
  auto cost_time = std::chrono::steady_clock::now() - before_time;
  std::cout << " total run cost " << std::chrono::duration_cast<std::chrono::milliseconds>(cost_time).count() << " ms" << std::endl;

  //打印操作
  struct in_addr in_addr_;
  std::vector<std::pair<int,int>> ipsize;
  for(auto &[ip, size] : ipd.ip_packet_size_) {
    ipsize.emplace_back(size, ip);
  }

  std::sort(ipsize.begin(), ipsize.end(), [&](const std::pair<int,int>& a, const std::pair<int,int>& b){
    return a.first > b.first;
  });

  for(auto & ip : ipsize) {
    in_addr_.s_addr = ip.second;
    double total_size = (double)ip.first / (1024*1024);
    if(total_size > 1)
      std::cout << inet_ntoa(in_addr_) << " " << (double)ip.first / (1024*1024) << "MiB" << std::endl;
  }
}
