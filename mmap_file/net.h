#pragma once
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <utility>
#include <bits/endian.h>
#include <arpa/inet.h>
#pragma pack(1)

typedef struct eth_rec {
	uint8_t           offset;
	uint8_t           pad;
//	uint8_t           dst[6];
//	uint8_t           src[6];
//	uint16          etype;
} eth_rec_t;

typedef struct flags {
	uint8_t           iface:2;
	uint8_t           vlen:1;
	uint8_t           trunc:1;
	uint8_t           rxerror:1;
	uint8_t           dserror:1;
	uint8_t           reserved:1;
	uint8_t           direction:1;
} flags_t;

typedef union {
	eth_rec_t	eth;
} erf_payload_t;

typedef struct erf_record {
	uint64_t          ts;
	uint8_t           type;
	flags_t           flags;
	uint16_t          rlen;
	uint16_t          lctr;
	uint16_t          wlen;
	erf_payload_t rec; 
} erf_record_t;

#pragma pack()

enum ErrCode
{
  KDecodeSuccess = 0,
	KHeadNotVaild,												//ehternet + ip + tcp headlen
	KNotVlanProtocol,									 //no exist vlan protocol
	KNotIpProtocol,										//no exist ip protocol
	KNotTcpProtocol,									//no exist tcp protocol
};

const int KEthernetLen = 14;
const int KVlanLen = 4;
const int KIpHeadLen = 20; // ignore options and padding
const int KTcpHeadLen = 20; // ignore options and padding

const int KVlanProtocol = 0x0081;
const int KIpProtocol = 0x0008;
const int KTcpProtocol = 0x0006;

#pragma pack(push, 1)
struct EthernetHeader
{
  uint8_t src_mac[6];
  uint8_t dst_mac[6];
  uint16_t protocol;
};

struct VlanHeader
{
  uint16_t vlan_tci;
  uint16_t protocol;
};

struct IpHeader
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
  uint8_t head_len:4;
  uint8_t version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
  uint8_t version:4;
  uint8_t head_len:4;
#else
#error "Adjust your <bits/endian.h> defines"
#endif
  uint8_t service;
  uint16_t total_length;
  uint16_t identifiler;
  uint16_t flag_off;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t src_ip;
  uint32_t dst_ip;
};

#pragma pack(pop)

struct Protocol
{
  EthernetHeader* ethernet{nullptr};
  VlanHeader* vlheader{nullptr};
  IpHeader * ipheader{nullptr};
};

inline static std::pair<Protocol, ErrCode> decode(uint8_t* buffer, uint32_t buffer_len)
{
  Protocol protocol{};
  uint32_t decode_len = 0;
  if(buffer_len < KEthernetLen + KVlanLen + KIpHeadLen + KTcpHeadLen )
    return {protocol,KHeadNotVaild};
  protocol.ethernet = (EthernetHeader*)(buffer + decode_len);
  if(protocol.ethernet->protocol != KVlanProtocol)
    return {protocol, KNotVlanProtocol};
  decode_len += KEthernetLen;
  protocol.vlheader = (VlanHeader*)(buffer + decode_len);
  if(protocol.vlheader->protocol != KIpProtocol)
    return {protocol, KNotIpProtocol};
  decode_len += KVlanLen;
  protocol.ipheader = (IpHeader*)(buffer + decode_len);
  return {protocol, KDecodeSuccess};
}
