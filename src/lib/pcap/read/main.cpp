#include "pcap_file.h"
#include <iostream>

int main()
{
  Pcap pcap("test.pcap");
  std::vector<std::string> data;
  pcap.ParsePcapFile(data);
  for(int i = 0; i < data.size(); i++) {
    std::cout << data[i] << std::endl;
  }
  return 0;
}

