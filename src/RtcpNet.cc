#include "RtcpNet.h"

rtcp_type_t int2rtcp_type(uint32 pt)
{
	switch(pt)
	{
	case 200:
		return RTCP_SR;
	case 201:
		return RTCP_RR;
	case 202:
		return RTCP_SDES;
	case 203:
		return RTCP_BYE;
	case 204:
		return RTCP_APP;
	case 205:
		return RTCP_NACK;
	default:
		return RTCP_UNKOWN;
	}
}
RtcpNet::RtcpNet(Packet& pack):
	Protocol(pack),
	rtcp_packet(NULL),
	sdes(NULL),app(NULL),
	bye(NULL),
	sender_report(NULL),
	reciever_report(NULL),
	nack(NULL)
{
	eProto = RTCP;
}
RtcpNet::RtcpNet(Protocol* obj):
	Protocol(obj),
	rtcp_packet(NULL),
	sdes(NULL),
	app(NULL),
	bye(NULL),
	sender_report(NULL),
	reciever_report(NULL),
	nack(NULL)
{
	eProto = RTCP;
}
uint32 RtcpNet::GetVersion()
{
	uint32 version = (uint32)(rtcp_packet->common.version);
	return version;
}
rtcp_type_t RtcpNet::GetRtcpType()
{
	return rtcp_type;
}
bool RtcpNet::IsPadding()
{
	bool padding = rtcp_packet->common.p > 0;
	return padding;
}
uint32 RtcpNet::GetLength()
{
	uint32 length = (uint32)SWAP16(rtcp_packet->common.length);
	return length;
}
bool RtcpNet::Parse()
{
	uint32 length;
	if(!CheckBuff())
		return false;
	rtcp_packet = (rtcp_t*)pstBuffer->GetBuffer(&length);
	rtcp_type = int2rtcp_type(rtcp_packet->common.pt);
	switch (rtcp_type)
	{
	case RTCP_SR:
		sender_report = new Rtcp_Sr(this);
		break;
	case RTCP_RR:
		reciever_report = new Rtcp_Rr(this);
		break;
	case RTCP_SDES:
		sdes = new Rtcp_Sdes(this);	
		break;
	case RTCP_BYE:
		bye = new Rtcp_Bye(this);
		break;
	case RTCP_APP:
		app = new Rtcp_App(this);
		break;
	case RTCP_NACK:
		nack = new Rtcp_Nack(this);
		break;
	default:
		return false;
	}
	//解析复合包
	if(length > (GetLength() + 1) * 4)
	{
		next = new	RtcpNet(this);
		next->SetOffSet(uiOffset + (GetLength() + 1) * 4);
		if(next->Parse())
		{
			delete next;
			return false;
		}
	}
	return true;
}
Protocol* RtcpNet::GetUpperProtocol()
{
	return pstPostProtcol;
}
Protocol* RtcpNet::GetLowerProtocol()
{
	return pstPreProtcol;
}
RtcpNet* RtcpNet::GetNext()
{
	return next;
}
Rtcp_Sr* RtcpNet::GetSenderReport()
{
	return this->sender_report;
}
Rtcp_Rr* RtcpNet::GetRecieverReport()
{
	return this->reciever_report;
}
Rtcp_Sdes* RtcpNet::GetSdes()
{
	return this->sdes;
}
Rtcp_App* RtcpNet::GetAppPacket()
{
	return this->app;
}
Rtcp_Nack* RtcpNet::GetNack()
{
	return this->nack;
}
Rtcp_Bye* RtcpNet::GetByePacket()
{
	return this->bye;
}
void RtcpNet::PrintInfo()
{
	printf("version: %d\n", GetVersion());
	printf("padding: %d\n", IsPadding());
	printf("report_count: %d\n", rtcp_packet->common.count);
	printf("rtcp_type: %d\n", GetRtcpType());
	printf("length: %d\n", GetLength());
}
rtcp_t* RtcpNet::GetHeader()
{
	return rtcp_packet;
}
RtcpNet::~RtcpNet(void)
{
	if(sender_report != NULL)
	{
		delete sender_report;
		sender_report = NULL;
	}
	if(reciever_report != NULL)
	{
		delete reciever_report;
		reciever_report = NULL;
	}
	if(sdes != NULL)
	{
		delete sdes;
		sdes = NULL;
	}
	if(app != NULL)
	{
		delete app;
		app = NULL;
	}
	if(nack != NULL)
	{
		delete nack;
		nack = NULL;
	}
	if(bye != NULL)
	{
		delete bye;
		bye = NULL;
	}
}
/************************RTCP Function End*****************************/
Rtcp_Rr::Rtcp_Rr(RtcpNet* rtcp)
{
	rr = &(rtcp->GetHeader()->r.rr);
	this->pstRtcp = rtcp;
}
uint32 Rtcp_Rr::GetSsrc()
{
	uint32 ssrc = (uint32)(rr->ssrc);
	return ssrc;
}
uint32 Rtcp_Rr::GetReportCount()
{
	uint32 count = (uint32)(pstRtcp->GetHeader()->common.count);
	return count;
}
rtcp_rr_t* Rtcp_Rr::GetReportBlockByIndex(uint32 index)
{
	rtcp_rr_t* block = NULL;
	if(index < GetReportCount())
	{
		block = rr->rr;
		block += index;
	}
	return block;
}
/************************RR Function End*****************************/
Rtcp_Sr::Rtcp_Sr(RtcpNet* rtcp)
{
	sr = &(rtcp->GetHeader()->r.sr);
	this->pstRtcp = rtcp;
}
uint32 Rtcp_Sr::GetReportCount()
{
	uint32 count = (uint32)(pstRtcp->GetHeader()->common.count);
	return count;
}
uint32 Rtcp_Sr::GetSsrc()
{
	uint32 ssrc = (uint32)(sr->ssrc);
	return ssrc;
}
uint32 Rtcp_Sr::GetNtpSec()
{
	uint32 ntp_sec = (uint32)(sr->ntp_sec);
	return ntp_sec;
}
uint32 Rtcp_Sr::GetNtpFrac()
{
	uint32 ntp_frac = (uint32)(sr->ntp_frac);
	return ntp_frac;
}
uint32 Rtcp_Sr::GetTimestamp()
{
	uint32 time_stamp = (uint32)(sr->rtp_ts);
	return time_stamp;
}
uint32 Rtcp_Sr::GetPacketsCount()
{
	uint32 packets_count = (uint32)(sr->psent);
	return packets_count;
}
uint32 Rtcp_Sr::GetOctetsCount()
{
	uint32 oct_count = (uint32)(sr->osent);
	return oct_count;
}
rtcp_rr_t* Rtcp_Sr::GetReportBlockByIndex(uint32 index)
{
	rtcp_rr_t* block = NULL;
	if(index < GetReportCount())
	{
		block = sr->rr;
		block += index;
	}
	return block;
}
/************************SR Function End*****************************/
Rtcp_Sdes::Rtcp_Sdes(RtcpNet* rtcp)
{
	sdes = &(rtcp->GetHeader()->r.sdes);
	this->pstRtcp = rtcp;
	for(int i = 0; i < 256; i++)
	{
		items[i] = NULL;
	}
	items_count = 0;
	Parse();
}
void Rtcp_Sdes::Parse()
{
	uint8* temp = sdes->sdes_items;
	uint32 length = (pstRtcp->GetHeader()->common.length - 1) * 4;
	uint32 offset = 0;
	while(offset < length && temp[offset] != '\0')
	{
		rtcp_sdes_item_t* item = (rtcp_sdes_item_t*)(temp + offset);
		items[items_count++] = item;
		offset += 2 + item->length;
	}
}
uint32 Rtcp_Sdes::GetSdesItemCount()
{
	return items_count;
}
uint32 Rtcp_Sdes::GetChunkCount()
{
	uint32 count = (uint32)(pstRtcp->GetHeader()->common.count);
	return count;
}
rtcp_sdes_item_t* Rtcp_Sdes::GetSdesItemByIndex(uint32 index)
{
	if(index < items_count)
		return items[index];
	return NULL;
}
/************************Sdes Function End*****************************/
Rtcp_Bye::Rtcp_Bye(RtcpNet* rtcp)
{
	this->pstRtcp = rtcp;
}
/************************Bye Function End*****************************/
Rtcp_App::Rtcp_App(RtcpNet* rtcp){}
/************************APP Function End*****************************/
Rtcp_Nack::Rtcp_Nack(RtcpNet* rtcp): query_count(0)
{
	this->pstRtcp = rtcp;
	nack = &(pstRtcp->GetHeader()->r.nack);
	for(int i = 0; i < 16; i++)
	{
		if(nack->query_list >> i && 0x01)
		{
			query_list[query_count++] = GetFirstQuerySN() + i;
		}
	}
	query_count++;//加上基准的SN
}
uint32 Rtcp_Nack::GetFMT()
{
	uint32 fmt = (uint32)(pstRtcp->GetHeader()->common.count);
	return fmt;
}
uint32 Rtcp_Nack::GetQueryCount()
{
	return query_count;
}
uint32 Rtcp_Nack::GetFirstQuerySN()
{
	uint32 first_sn = (uint32)SWAP16(nack->sn);
	return first_sn;
}
uint32 Rtcp_Nack::GetQuerySNByIndex(uint32 index)
{
	if(index >= query_count)
		return 0;
	if(index == 0)
		return GetFirstQuerySN();
	return query_list[index - 1];
}
uint32 Rtcp_Nack::GetLocalSSRC()
{
	uint32 local_ssrc = SWAP32(nack->ssrc);
	return local_ssrc;
}
uint32 Rtcp_Nack::GetRemoteSSRC()
{
	uint32 remote_ssrc = SWAP32(nack->remote_ssrc);
	return remote_ssrc;
}
/************************Nack Function End*****************************/