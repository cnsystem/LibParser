#pragma once
#include "protocol.h"
class EtherNet :
	public Protocol
{
public:
	EtherNet(Packet& pack);
	EtherNet(Protocol* obj);

	int8* GetSrcHost();
	int8* GetDstHost();

	void SetSrcHost(int8* srcHost);
	void SetDstHost(int8* dstHost);

	virtual bool Parse();
	virtual Protocol* GetUpperProtocol();
	virtual Protocol* GetLowerProtocol();
	virtual void PrintInfo();
	~EtherNet(void);
protected:
	eth_header* header;
	int8 srcAddr[256];
	int8 dstAddr[256];
	void hex2str(int8* str, uint8* buffer, uint32 len);
};

