#include "pcap_file.h"

Pcap::Pcap(const char* file_name)
{
  file_name_ = file_name;
}

void  Pcap::ParsePcapFile(std::vector<std::string>&  raw_vec)
{
  PcapFileHeader_t  pcap_fileheader{};
  PcapPkthdr_t  packet_header{};

  file_handler_.open(file_name_);
  if (!file_handler_)
  {
    std::cout << "The file does not exits or file name is error" << std::endl;
    return;
  }
  //读取pcap文件头部长度
  file_handler_.read(reinterpret_cast<char *>(&pcap_fileheader), sizeof(pcap_fileheader));
  if (pcap_fileheader.magic[0] != PCAP_FILE_MAGIC_1 || pcap_fileheader.magic[1] != PCAP_FILE_MAGIC_2 ||
      pcap_fileheader.magic[2] != PCAP_FILE_MAGIC_3 || pcap_fileheader.magic[3] != PCAP_FILE_MAGIC_4)
  {
    std::cout << "The file is not a pcap file" << std::endl;
    return;
  }

  while (file_handler_.read(reinterpret_cast<char *>(&packet_header), sizeof(packet_header)))
  {
    uint32_t len = packet_header.caplen;
    if (packet_header.caplen != packet_header.len)
    {
      continue;
    }

    char *buf = new char[len];
    if (NULL == buf)
    {
        return;
    }
    file_handler_.read(buf, len);
    std::string temp(buf, len);
    raw_vec.emplace_back(temp);
    delete buf;
  }
}

Pcap::~Pcap()
{
    file_handler_.close();
}
