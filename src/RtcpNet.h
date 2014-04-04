#pragma once
#include "protocol.h"

typedef enum {
	RTCP_SR   = 200,
	RTCP_RR   = 201,
	RTCP_SDES = 202,
	RTCP_BYE  = 203,
	RTCP_APP  = 204,
	RTCP_NACK = 205,
	RTCP_UNKOWN = 0xFF
} rtcp_type_t;
/* RTCP common header word */
typedef struct {
	uint32 count:5;     /* varies by packet type */
	uint32 p:1;         /* padding flag */
	uint32 version:2;   /* protocol version */
	uint32 pt:8;        /* RTCP packet type */
	uint32 length:16;           /* pkt len in words, w/o this word */
} rtcp_common_t;
/* Reception report block */
typedef struct {
	uint32 ssrc;             /* data source being reported */
	uint32 fraction:8;		/* fraction lost since last SR/RR */
	uint32  lost:24;			/* cumul. no. pkts lost (signed!) */
	uint32 last_seq;         /* extended last seq. no. received */
	uint32 jitter;           /* interarrival jitter */
	uint32 lsr;              /* last SR packet from this source */
	uint32 dlsr;             /* delay since last SR packet */
} rtcp_rr_t;
/* SDES item */
typedef struct {
	uint8 type;              /* type of item (rtcp_sdes_type_t) */
	uint8 length;            /* length of item (in octets) */
	uint8 data[0];             /* text, not null-terminated */
} rtcp_sdes_item_t;
typedef struct {
	uint32 ssrc;		/* sender generating this report */
	uint32 ntp_sec;		/* NTP timestamp */
	uint32 ntp_frac;
	uint32 rtp_ts;		/* RTP timestamp */
	uint32 psent;		/* packets sent */
	uint32 osent;		/* octets sent */
	rtcp_rr_t rr[1];	/* variable-length list */
} send_report;			/* sender report (SR) */
typedef struct {
	uint32 ssrc;		/* receiver generating this report */
	rtcp_rr_t rr[1];	/* variable-length list */
} reciver_report;		/* reception report (RR) */
typedef struct {
	uint32 ssrc;			/* first SSRC/CSRC */
	uint8 sdes_items[0]; /* list of SDES items */
} rtcp_sdes;			/* source description (SDES) */
typedef struct {
	uint32 ssrc[1];		/* list of sources */
	/* can't express trailing text for reason */
} rtcp_bye;/* BYE */
typedef struct{
	uint32 ssrc;
	uint32 remote_ssrc;
	uint16 sn;
	uint16 query_list;
}rtcp_nack;
/** One RTCP packet */
typedef struct {
	rtcp_common_t common;     /* common header */
	union {
		send_report sr;
		reciver_report rr;
		rtcp_sdes sdes; 
		rtcp_bye bye;
		rtcp_nack nack;
	} r;
} rtcp_t;
class Rtcp_Sdes;
class Rtcp_App;
class Rtcp_Bye;
class Rtcp_Sr;
class Rtcp_Rr;
class Rtcp_Nack;
class RtcpNet :
	public Protocol
{
public:
	RtcpNet(Packet& pack);
	RtcpNet(Protocol* obj);

	uint32 GetVersion();
	rtcp_type_t GetRtcpType();
	//uint32 GetRtcpCount();
	bool IsPadding();
	uint32 GetLength();
	virtual void PrintInfo();
	rtcp_t* GetHeader();
	Rtcp_Sr* GetSenderReport();
	Rtcp_Rr* GetRecieverReport();
	Rtcp_Sdes* GetSdes();
	Rtcp_App* GetAppPacket();
	Rtcp_Nack* GetNack();
	Rtcp_Bye* GetByePacket();
	virtual bool Parse();
	virtual Protocol* GetUpperProtocol();
	virtual Protocol* GetLowerProtocol();
	RtcpNet* GetNext();
	~RtcpNet(void);
protected:
	RtcpNet* next;
	rtcp_type_t rtcp_type;
	rtcp_t* rtcp_packet;

	Rtcp_Sr *sender_report;
	Rtcp_Rr *reciever_report;
	Rtcp_Sdes *sdes;
	Rtcp_App *app;
	Rtcp_Nack *nack;
	Rtcp_Bye *bye;
};
class Rtcp_Rr
{
public:
	Rtcp_Rr(RtcpNet* rtcp);
	uint32 GetSsrc();
	uint32 GetReportCount();
	rtcp_rr_t* GetReportBlockByIndex(uint32 index);
protected:
	reciver_report* rr;
	RtcpNet* pstRtcp;
};
class Rtcp_Sr
{
public:
	Rtcp_Sr(RtcpNet* rtcp);
	uint32 GetReportCount();
	uint32 GetSsrc();
	uint32 GetNtpSec();
	uint32 GetNtpFrac();
	uint32 GetTimestamp();
	uint32 GetPacketsCount();
	uint32 GetOctetsCount();
	rtcp_rr_t* GetReportBlockByIndex(uint32 index);
protected:
	send_report* sr;
	RtcpNet* pstRtcp;
};
class Rtcp_App
{
public:
	Rtcp_App(RtcpNet* rtcp);
};
class Rtcp_Bye
{
public:
	Rtcp_Bye(RtcpNet* rtcp);
	uint32 GetSsrclist(uint32* ssrc);
protected:
	RtcpNet* pstRtcp;
};
class Rtcp_Sdes
{
public:
	Rtcp_Sdes(RtcpNet* rtcp);
	uint32 GetChunkCount();
	uint32 GetSdesItemCount();
	rtcp_sdes_item_t* GetSdesItemByIndex(uint32 index);
protected:
	void Parse();
	rtcp_sdes* sdes;
	uint32 items_count;
	rtcp_sdes_item_t* items[256];
	RtcpNet* pstRtcp;
};

class Rtcp_Nack
{
public:
	Rtcp_Nack(RtcpNet* rtcp);
	uint32 GetFMT();
	uint32 GetQueryCount();
	uint32 GetFirstQuerySN();
	uint32 GetQuerySNByIndex(uint32 index);
	uint32 GetLocalSSRC();
	uint32 GetRemoteSSRC();
protected:
	uint16 query_list[16];
	rtcp_nack* nack;
	uint32 query_count;
	RtcpNet* pstRtcp;
};
