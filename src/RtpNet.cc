#include "RtpNet.h"


RtpNet::RtpNet(Packet& pack):Protocol(pack),header(NULL),extend(NULL),padding_count(0)
{
	eProto = RTP;
	uiHeaderLen = sizeof(rtp_header);
	for(int i = 0; i < 16; i++)
	{
		csrc_list[i] = NULL;
	}
}
RtpNet::RtpNet(Protocol* obj):Protocol(obj),header(NULL),extend(NULL),padding_count(0)
{
	eProto = RTP;
	uiHeaderLen = sizeof(rtp_header);
	for(int i = 0; i < 16; i++)
	{
		csrc_list[i] = NULL;
	}
}
uint32 RtpNet::GetVersion()
{
	uint32 version = (uint32)(header->ver_p_x_cc >> 6);
	return version;
}
bool RtpNet::IsPadding()
{
	//0x20 = 0010 0000
	return (header->ver_p_x_cc & 0x20) > 0;
}
bool RtpNet::IsExtend()
{
	//0x10 = 0001 0000
	return (header->ver_p_x_cc & 0x10) > 0;
}
uint32 RtpNet::GetCsrcCount()
{
	uint32 csrc_count = (uint32)(header->ver_p_x_cc & 0x0F);
	return csrc_count;
}
bool RtpNet::IsMark()
{
	//0x80 = 1000 0000
	return (header->m_pt & 0x80) > 0;
}
uint32 RtpNet::GetPayloadType()
{
	//0x7F = 0111 1111
	uint32 payload_type = (uint32)(header->m_pt & 0x7F);
	return payload_type;
}
uint32 RtpNet::GetSerialNumber()
{
	uint32 serial_number = (uint32)(SWAP16(header->sn));
	return serial_number;
}
uint32 RtpNet::GetTimespan()
{
	uint32 time_span = (uint32)(SWAP32(header->ts));
	return time_span;
}
uint32 RtpNet::GetSsrc()
{
	uint32 ssrc = (uint32)(SWAP32(header->ssrc));
	return ssrc;
}
bool RtpNet::Parse()
{
	uint32 length;
	uint8* temp;
	if(!CheckBuff())
		return false;
	header = (rtp_header*)pstBuffer->GetBuffer(&length);
	//1. 解析CSRC
	if(GetCsrcCount() > 0)
	{
		temp = (uint8*)header + uiHeaderLen;
		for (uint32 i = 0; i < GetCsrcCount(); i++)
		{
			csrc_list[i] = (uint32*)temp;
			temp += sizeof(uint32);
		}
		uiHeaderLen += GetCsrcCount() * 4;
	}
	//2. 解析Extending部分
	if(IsExtend())
	{
		temp = (uint8*)header + uiHeaderLen;
		extend = (ext_header*) temp;
		uiHeaderLen += extend->length;
	}
	//3. 解析Padding部分
	if(IsPadding())
	{
		temp = (uint8*)header;
		padding_count = (uint32)temp[length];
	}
	//对负载偏移从新计算
	pstPayload = pstBuffer->GetBufferByOffSet(uiHeaderLen, padding_count-1);
	if(!pstPayload)
		return false;
	return true;
}
Protocol* RtpNet::GetUpperProtocol()
{
	return pstPostProtcol;
}
Protocol* RtpNet::GetLowerProtocol()
{
	return pstPreProtcol;
}
void RtpNet::PrintInfo()
{
	printf("version: %d\n", GetVersion());
	printf("padding: %d\n", IsPadding());
	printf("extend length: %d\n", IsExtend());
	printf("csrc count: %d\n", GetCsrcCount());
	printf("mark: %d\n", IsMark());
	printf("pt: %d\n", GetPayloadType());
	printf("sn: %d\n", GetSerialNumber());
	printf("ts: %d\n", GetTimespan());
	printf("ssrc: %d\n", GetSsrc());
}
RtpNet::~RtpNet(void)
{
}
