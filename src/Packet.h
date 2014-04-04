#pragma once
#include "Buffer.h"
class Protocol;
class EtherNet;
class IpNet;
class UdpNet;
class RtpNet;
class RtcpNet;
class Packet
{
public:
	Packet(Buffer* buf);
	
	~Packet(void);

	void SetBuffer(Buffer* buf);
	Buffer* GetBuffer();
	Buffer* GetData();
	time_val GetCapTime();
	uint32 GetPaketLength();
	uint32 GetDataLength();
	EtherNet* GetEtherPacket();
	IpNet* GetIpNet();
	UdpNet* GetUdpNet();
	RtpNet* GetRtpNet();
	RtcpNet* GetRtcpNet();
	void Parse();
	void PrintInfo();
	Packet* next;
	Packet* pre;
private:
	Buffer* buffer; 
	Buffer* data;
	packet_hdr* header;
	Protocol* protoStack;
};

