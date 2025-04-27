#include <iostream>
#include <string>
#include <cstring>
#include <pcap.h>

#define PKT_CORE(pkt) (pkt->core_id_)
#define PKT_PORT(pkt) (pkt->port_id_)
#define PKT_QUEUE(pkt) (pkt->queue_id_)
#define PKT_CAPLEN(pkt) (pkt->cap_len_)
#define PKT_DATA_LEN(pkt) (pkt->data_len_le_)
#define PKT_RSS(pkt) (pkt->rss_)
#define PKT_TIME(pkt) (pkt->ts_)
#define PKT_DATA(pkt, type) ((type)((char *)pkt + sizeof(RcvPacket)))
#define PKT_WRITE_DATA(pkt, type)((type)((char *)pkt + offsetof(RcvPacket, pcap_hdr_)))
#define PKT_WRITE_LEN(pkt) (pkt->cap_len_ + sizeof(pcap_pkthdr))

struct RcvPacket {
  char padding[40];
  uint8_t core_id_;
  uint8_t port_id_;
  uint8_t queue_id_;
  uint16_t cap_len_;
  uint16_t data_len_le_;
  uint32_t rss_;
  uint64_t ts_;
  struct pcap_pkthdr pcap_hdr_;
  /* void * data */
};

int main()
{
  RcvPacket packet;
  packet.pcap_hdr_.caplen = 1111;
  packet.pcap_hdr_.len = 22222;
  pcap_pkthdr * hdr = PKT_WRITE_DATA(&packet, pcap_pkthdr *);
  std::cout << hdr->caplen << "  " << hdr->len << std::endl;
  return 0;
}
