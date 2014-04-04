#pragma once
#include "protocol.h"
class IpNet :
	public Protocol
{
public:
	IpNet(Packet& pack);
	IpNet(Protocol* obj);

	uint32 GetVersion();
	uint32 GetTosValue();
	uint32 GetTotalLength();
	uint32 GetFlags();
	uint32 GetSliceOffset();
	uint32 GetTtl();
	proto_type GetProtoType();
	uint32 GetCRC();

	int8* GetSrcIpAddr();
	int8* GetDstIpAddr();

	virtual bool Parse();
	virtual Protocol* GetUpperProtocol();
	virtual Protocol* GetLowerProtocol();
	virtual void PrintInfo();
	~IpNet(void);
protected:
	ip_header* header;
	int8 srcAddr[16];
	int8 dstAddr[16];
	void hex2str(int8* str, uint8* buffer, uint32 len);
};

