 /* types.h, v 1.0 2013/08/24 10:10:44
  * 
  * PacketParse - Network traffic analyzer
  * By Gerald Billy Zheng <cnsystem@yeah.net>
  * Copyright 2013 Billy Zheng
  *
  * This file defines common struct for this Program.
  * 此头文件定义了一些常用协议的头部结构体
  */

#ifndef TYPES_H
#define TYPES_H
#define DO_SWAP
#ifdef DO_SWAP
#define SWAP16(x) \
		  (((x & 0x00ff) << 8) | \
		  ((x & 0xff00) >> 8))

#define SWAP32(x) \
		  (((x & 0x000000ff) << 24) | \
		  ((x & 0xff000000) >> 24) | \
		  ((x & 0x00ff0000) >> 8) | \
		  ((x & 0x0000ff00) << 8))
#else
#define SWAP16(x) (x)
#define SWAP32(x) (x)
#endif /* DO_SWAP */

typedef unsigned char byte;
typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;

#define ETH_ADDR_LEN 6
#define ETH_HEADER_LEN 14
#define IP_HEADER_LEN 20
#define UDP_HEADER_LEN 8
#define RTP_HEADER_LEN 12

typedef enum _proto_type
{
	UNKOWN = 0xFF,
	ETHER_NET = 0x00,
	IP_NET = 0x10,
	UDP = 0x20,
	TCP = 0x21,
	RTP = 0x30,
	RTCP = 0x31
}proto_type;

typedef struct eth_header {
	uint8	ether_dhost[ETH_ADDR_LEN];
	uint8	ether_shost[ETH_ADDR_LEN];
	uint16	ether_type;
}eth_header;

typedef struct ip_header {
	uint8	ver_ihl;
	uint8	tos;
	uint16	tlen;
	uint16	iden;
	uint16	flags_offset;
	uint8	ttl;
	uint8	proto;
	uint16	ip_crc;
	uint32	src_addr;
	uint32	dst_addr;
}ip_header;

typedef struct udp_header {
	uint16	src_port;
	uint16	dst_port;
	uint16	udp_len;
	uint16	udp_crc;
}udp_header;

typedef struct rtp_header {
	uint8	ver_p_x_cc;
	uint8	m_pt;
	uint16	sn;
	uint32	ts;
	uint32	ssrc;
}rtp_header;

typedef struct time_val
{
	uint32	tv_sec;
	uint32	tv_usec;
}time_val;

typedef struct packet_hdr
{
	struct time_val	ts;
    uint32	caplen;
    uint32	len;
}packet_hdr;
 

#endif