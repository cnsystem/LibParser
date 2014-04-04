#include "UdpNet.h"
#include "RtpNet.h"
#include "RtcpNet.h"

UdpNet::UdpNet(Packet& pack):Protocol(pack),header(NULL)
{
	eProto = UDP;
	uiHeaderLen = sizeof(udp_header);
}
UdpNet::UdpNet(Protocol* obj):Protocol(obj),header(NULL)
{
	eProto = UDP;
	uiHeaderLen = sizeof(udp_header);
}

uint32 UdpNet::GetSrcPort()
{
	uint32 src_port = (uint32)(SWAP16(header->src_port));
	return src_port;
}
uint32 UdpNet::GetDstPort()
{
	uint32 dst_port = (uint32)(SWAP16(header->dst_port));
	return dst_port;
}
uint32 UdpNet::GetLength()
{
	uint32 length = (uint32)(SWAP16(header->udp_len));
	return length;
}
uint32 UdpNet::GetCRC()
{
	uint32 crc = (uint32)(SWAP16(header->udp_crc));
	return crc;
}

bool UdpNet::Parse()
{
	uint32 length;
	if(!CheckBuff())
		return false;
	header = (udp_header*)pstBuffer->GetBuffer(&length);
	return true;
}
Protocol* UdpNet::GetUpperProtocol()
{
	if(pstPostProtcol == NULL)
	{
		////1. try to parse as RTCP
		//pstPostProtcol = new RtcpNet(this);
		//if(pstPostProtcol->Parse())
		//{
		//	pstPostProtcol->SetOffSet(uiOffset + uiHeaderLen);
		//	return pstPostProtcol;
		//}
		//delete pstPostProtcol;
		////2. try to parse as RTP
		//pstPostProtcol = new RtpNet(this);
		//if(pstPostProtcol->Parse())
		//{
		//	pstPostProtcol->SetOffSet(uiOffset + uiHeaderLen);
		//	return pstPostProtcol;
		//}
		//delete pstPostProtcol;		
		//3. Parse it according port
		if(header->dst_port % 2 == 1)
		{
			pstPostProtcol = new RtpNet(this);
		}
		else
		{
			pstPostProtcol = new RtcpNet(this);
		}
	}
	pstPostProtcol->SetOffSet(uiOffset + uiHeaderLen);
	return pstPostProtcol;
}
Protocol* UdpNet::GetLowerProtocol()
{
	return pstPreProtcol;
}

void UdpNet::PrintInfo()
{
	printf("src port: %d\n", GetSrcPort());
	printf("dst port: %d\n", GetDstPort());
	printf("total length: %d\n", GetLength());
	printf("crc: %x\n", GetCRC());
}
UdpNet::~UdpNet(void)
{
}
