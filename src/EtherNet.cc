#include "EtherNet.h"
#include "StringHelper.h"
#include "IpNet.h"
EtherNet::EtherNet(Packet& pack):Protocol(pack),header(NULL)
{
	eProto = ETHER_NET;
}

EtherNet::EtherNet(Protocol* obj):Protocol(obj),header(NULL)
{
	eProto = ETHER_NET;
	uiHeaderLen = sizeof(eth_header);
}

void EtherNet::hex2str(int8* str, uint8* buffer, uint32 len)
{
	uint32 i;
	for(i = 0; i<len; i++)
	{
		StringHelper::byte2str(buffer[i], str[3*i], str[3*i+1]);
		str[3*i+2] = '-';
	}
	str[3*i-1] = '\0';
}

int8* EtherNet::GetSrcHost()
{
	return srcAddr;
}
int8* EtherNet::GetDstHost()
{
	return dstAddr;
}
void EtherNet::SetSrcHost(int8* srcHost)
{

}
void SetDstHost(int8* dstHost)
{

}

bool EtherNet::Parse()
{
	uint32 length;
	if(!CheckBuff())
		return false;
	header = (eth_header*)pstBuffer->GetBuffer(&length);
	hex2str(srcAddr, header->ether_shost, sizeof(header->ether_shost));
	hex2str(dstAddr, header->ether_dhost, sizeof(header->ether_dhost));
	return true;
}
Protocol* EtherNet::GetUpperProtocol()
{
	if(pstPostProtcol == NULL)
	{
		pstPostProtcol = new IpNet(this);
		pstPostProtcol->SetOffSet(uiOffset + uiHeaderLen);
	}
	return pstPostProtcol;
}
Protocol* EtherNet::GetLowerProtocol()
{
	return pstPreProtcol;
}

void EtherNet::PrintInfo()
{
	printf("src host: %s\n", GetSrcHost());
	printf("dst host: %s\n", GetDstHost());
}
EtherNet::~EtherNet(void)
{
}
