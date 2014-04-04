#include "Packet.h"
#include "Buffer.h"
#include "Protocol.h"
#include "EtherNet.h"
#include "IpNet.h"
#include "UdpNet.h"
#include "RtpNet.h"
#include "RtcpNet.h"

void PrintHead(packet_hdr* head)
{
	printf("  head->ts.tv_sec: %d \n", head->ts.tv_sec);
	printf("  head->ts.tv_sec: %d \n", head->ts.tv_usec);
	printf("  head->caplen: %d \n", head->caplen);
	printf("  head->len: %d \n", head->len);
}

Packet::Packet(Buffer* buf): next(NULL), pre(NULL)
{
	SetBuffer(buf);
}

void Packet::SetBuffer(Buffer* buf)
{
	uint32 len = 0;
	header = (packet_hdr*)buf->GetBuffer(&len);
	buffer = buf;
	data = buf->GetBufferByOffSet(sizeof(packet_hdr));
}
time_val Packet::GetCapTime()
{
	return header->ts;
}
uint32 Packet::GetPaketLength()
{
	return header->caplen;
}
uint32 Packet::GetDataLength()
{
	uint32 len = 0;
	byte* buf = data->GetBuffer(&len);
	return len;
}
void Packet::Parse()
{
	protoStack = new Protocol(*this);
	Protocol* temp = protoStack;
	while(temp->Parse() && temp->GetUpperProtocol())
	{
		temp->GetUpperProtocol()->SetLowerProtocol(temp);
		temp = temp->GetUpperProtocol();
	}
}
Buffer* Packet::GetBuffer()
{
	return buffer;
}
Buffer* Packet::GetData()
{
	return data;
}
char* GetType(proto_type type)
{
	switch (type)
	{

	case ETHER_NET:
		return "ETHER_NET";
	case IP_NET:
		return "IP_NET";
	case UDP:
		return "UDP";
	case TCP:
		return "TCP";
	case RTP:
		return "RTP";
	case RTCP:
		return "RTCP";
	case UNKOWN:
	default:
		return "UNKOWN";
	}
}
void Packet::PrintInfo()
{
	Protocol* temp = protoStack;
	PrintHead(header);
	/*while(temp)
	{
		printf("*****Protocol: %s\n", GetType(temp->GetType()));
		temp->PrintInfo();
		temp = temp->GetUpperProtocol();
	}*/
}

EtherNet* Packet::GetEtherPacket()
{
	Protocol* temp = protoStack;
	while(temp)
	{
		if(temp->GetType() == ETHER_NET)
			break;
		temp = temp->GetUpperProtocol();
	}
	return (EtherNet*)temp;
}
IpNet* Packet::GetIpNet()
{
	Protocol* temp = protoStack;
	while(temp)
	{
		if(temp->GetType() == IP_NET)
			break;
		temp = temp->GetUpperProtocol();
	}
	return (IpNet*)temp;
}
UdpNet* Packet::GetUdpNet()
{
	Protocol* temp = protoStack;
	while(temp)
	{
		if(temp->GetType() == UDP)
			break;
		temp = temp->GetUpperProtocol();
	}
	return (UdpNet*)temp;
}
RtpNet* Packet::GetRtpNet()
{
	Protocol* temp = protoStack;
	while(temp)
	{
		if(temp->GetType() == RTP)
			break;
		temp = temp->GetUpperProtocol();
	}
	return (RtpNet*)temp;
}
RtcpNet* Packet::GetRtcpNet()
{
	Protocol* temp = protoStack;
	while(temp)
	{
		if(temp->GetType() == RTCP)
			break;
		temp = temp->GetUpperProtocol();
	}
	return (RtcpNet*)temp;
}
Packet::~Packet(void)
{
	Protocol* temp = protoStack;
	while(temp)
	{
		Protocol* p = temp; 
		temp = temp->GetUpperProtocol();
		delete p;
	}
	delete data;
}
