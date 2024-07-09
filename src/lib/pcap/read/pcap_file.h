#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#define  PCAP_FILE_MAGIC_1   0Xd4
#define  PCAP_FILE_MAGIC_2   0Xc3
#define  PCAP_FILE_MAGIC_3   0Xb2
#define  PCAP_FILE_MAGIC_4   0Xa1


/*pcap file header*/
typedef struct PcapFileHeader
{
    uint8_t   magic[4];
    uint16_t   version_major;
    uint16_t   version_minor;
    int32_t    thiszone;      /*时区修正*/
    uint32_t   sigfigs;       /*精确时间戳*/
    uint32_t   snaplen;       /*抓包最大长度*/
    uint32_t   linktype;      /*链路类型*/
} PcapFileHeader_t;


/*pcap packet header*/
typedef struct PcapPkthdr
{
    uint32_t   seconds;     /*秒数*/
    uint32_t   u_seconds;   /*毫秒数*/
    uint32_t   caplen;      /*数据包长度*/
    uint32_t   len;         /*文件数据包长度*/
} PcapPkthdr_t;

class Pcap
{
public:
    Pcap(const char* file_name);
    ~Pcap();
    void  ParsePcapFile(std::vector<std::string>&  raw_vec);

private:
    std::ifstream  file_handler_;
    const char*    file_name_;
};
