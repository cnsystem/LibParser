#pragma once
#include "protocol.h"
typedef struct ext_header
{
	uint16 profile;
	uint16 length;
}ext_header;
class RtpNet
	:public Protocol
{
public:
	RtpNet(Packet& pack);
	RtpNet(Protocol* obj);

	uint32 GetVersion();
	bool IsPadding();
	bool IsExtend();
	uint32 GetCsrcCount();
	bool IsMark();
	uint32 GetPayloadType();
	uint32 GetSerialNumber();
	uint32 GetTimespan();
	uint32 GetSsrc();

	virtual bool Parse();
	virtual Protocol* GetUpperProtocol();
	virtual Protocol* GetLowerProtocol(); 
	virtual void PrintInfo();
	uint32* csrc_list[16];
	ext_header* extend;
	uint32 padding_count;
	~RtpNet(void);
protected:
	rtp_header* header;
	
};

