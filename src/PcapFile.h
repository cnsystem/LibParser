#pragma once
#include <stdio.h>
#include "types.h"
#include "PacketArray.h"
typedef struct pcap_file_header
{
	uint32 magic; //标识位：32位的，这个标识位的值是16进制的 0xa1b2c3d4。
	uint16 version_major; //主版本号：16位， 默认值为0x2。
	uint16 version_minor; //副版本号：16位，默认值为0x04。
	uint32 thiszone; //区域时间：32位，实际上该值并未使用，因此可以将该位设置为0。
	uint32 sigfigs; //精确时间戳：32位，实际上该值并未使用，因此可以将该值设置为0。
	uint32 snaplen; //数据包最大长度：32位，该值设置所抓获的数据包的最大长度
	uint32 linktype; //链路层类型：32位， 数据包的链路层包头决定了链路层的类型
}pcap_file_header;
/*************************************************************************
//以下是数据值与链路层类型的对应表
//0          BSD       loopback devices, except for later OpenBSD
//1          Ethernet, and Linux loopback devices   以太网类型，大多数的数据包为这种类型。
//6          802.5 Token Ring
//7          ARCnet
//8          SLIP
//9          PPP
//10         FDDI
//100        LLC/SNAP-encapsulated ATM
//101        raw IP, with no link
//102        BSD/OS SLIP
//103        BSD/OS PPP
//104        Cisco HDLC
//105        802.11
//108        later OpenBSD loopback devices (with the AF_value in network byte order)
//113        special Linux cooked capture
//114        LocalTalk
*************************************************************************/


class PcapFile
{
public:
	static PcapFile* Open(char* filename);	
	int ReadFile(void);
	int Parse(void);
	uint32 GetPacketCount();
	Packet* GetPacket();
	PacketArray* GetPacketArray();
	~PcapFile(void);
protected:
	PacketArray *packList;
private:
	PcapFile(void);
	FILE* file;
	pcap_file_header header;
	int length;
	uint8* buffer;
};

