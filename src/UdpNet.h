#pragma once
#include "protocol.h"
class UdpNet :
	public Protocol
{
public:
	UdpNet(Packet& pack);
	UdpNet(Protocol* obj);

	uint32 GetSrcPort();
	uint32 GetDstPort();
	uint32 GetLength();
	uint32 GetCRC();

	virtual bool Parse();
	virtual Protocol* GetUpperProtocol();
	virtual Protocol* GetLowerProtocol();
	virtual void PrintInfo();
	~UdpNet(void);
protected:
	udp_header* header;
};

