// ParseDemo.cpp : 定义控制台应用程序的入口点。
//

#include <stdlib.h>
#include <string.h>
#include "Packet.h"
#include "types.h"
#include "PcapFile.h"
#include "RtcpNet.h"
#include "PacketArray.h"
#include "UdpNet.h"
#include "IpNet.h"
extern char    *optarg;
extern int     opterr;
extern int     optopt;
int getopt (int argc, char *const argv[], const char *opts);
typedef struct option
{
	int32 opt_name;
	char* value;
	int32 use_arg;
	const char* opt_desc;
}app_option;
static app_option options[] = {
	{ 'c', NULL, 1, "function command: get_max_packet_size, count_packet, compute_bandwidth, all" },
	{ 'f', NULL, 1, "packet file" },
	{ 'a', NULL, 0, "source ip address" },
	{ 'A', NULL, 0, "destination ip Address" },
	{ 'p', NULL, 0, "source port" },
	{ 'P', NULL, 0, "destination Port" },
};
void format_optstring_from_options(char* buffer)
{
	for(int i = 0; i < sizeof(options) / sizeof(app_option); i++)
	{
		buffer[2 * i] = (char)options[i].opt_name;
		buffer[2 * i + 1] = ':';
	}
}
void free_options_value()
{
	for(int i = 0; i < sizeof(options) / sizeof(app_option); i++)
	{
		if(options[i].use_arg)
			free(options[i].value);
	}
}
app_option* get_app_option_by_name(int32 opt_name)
{
	for(int i = 0; i < sizeof(options) / sizeof(app_option); i++)
	{
		if(options[i].opt_name == opt_name)
			return &options[i];
	}
	return NULL;
}
float ComputeBandwidth(PacketArray* arr)
{
	uint32 last_time = 0;
	float packet_size = 0;
	uint32 current_time = 0;
	for(uint32 i = 0; i < arr->GetCount(); i++)
	{
		Packet* p = arr->GetPacketByIndex(i);
		packet_size += p->GetDataLength();
		current_time = p->GetCapTime().tv_sec * 1000;
		current_time += p->GetCapTime().tv_usec / 1000;
		if(i == 0)
		{
			last_time = current_time;
			continue;
		}
	}
	float bandwitch = (packet_size * 8) / (current_time - last_time);
	return bandwitch;
}
uint32 GetMaxSize(PacketArray* arr)
{
	uint32 max_size = 0;
	Packet* p = arr->GetPacketByIndex(0);
	while(p)
	{
		if(p->GetDataLength() > max_size)
		{
			max_size = p->GetDataLength();
			p->PrintInfo();
		}
		p = p->next;		
	}
	return max_size;
}
bool _SelectPackets(Packet* pack)
{
	int count = 0;
	UdpNet* udp = pack->GetUdpNet();
	if(udp == NULL) 
		return false;
	app_option* opt = NULL;

	if( (opt = get_app_option_by_name('a'))->use_arg)
		if(!strcmp(opt->value, pack->GetIpNet()->GetSrcIpAddr()))
			return false;
	if( (opt = get_app_option_by_name('A'))->use_arg)
		if(!strcmp(opt->value, pack->GetIpNet()->GetDstIpAddr()))
			return false;
	if( (opt = get_app_option_by_name('p'))->use_arg)
		if(atol(opt->value) != pack->GetUdpNet()->GetSrcPort())
			return false;
	if( (opt = get_app_option_by_name('P'))->use_arg)
		if(atol(opt->value) != pack->GetUdpNet()->GetDstPort())
			return false;
	return true;
}

void ParsePacpFile(PcapFile* file)
{
	file->ReadFile();
	file->Parse();
	Packet* p = file->GetPacket();
	while(p)
	{
		p->Parse();
		p = p->next;		
	}
}
#define USEAGE() do {\
	printf("ParseDemo -[aApP] -c command -f filename\n");\
	for(int i = 0; i < sizeof(options) / sizeof(app_option); i++) \
	printf("    -%c: %s\n", options[i].opt_name, options[i].opt_desc);\
} while(0)

int main(int argc, char* argv[])
{
	int ch;  
	opterr = 0;  
	char optstring[256];
	app_option* opt = NULL;
	char filename[256];
	PcapFile* pcap;
	bool useSelect = false;
	SelectPacket selector = _SelectPackets;
#ifdef DEBUG
	char* file1 = "E:\\HME_Test\\Test_Vects\\out_pcap\\test_rtcp.pcap";
	char* file2 = "E:\\HME_Test\\Test_Vects\\test_pcap\\SA_NS_RTP_2198_001.pcap";
	char* file3 = "E:\\HME_Video\\Test_Vects\\test_pcap\\SPC_NET_RTP_VERSION_001.pcap";
#endif
	//1.0 解析参数
	format_optstring_from_options(optstring);	
	while ((ch = getopt(argc,argv, optstring))!=-1)  
	{  
		opt = get_app_option_by_name(ch);
		if(opt == NULL)
		{
			printf("the arguments input is error, please see the grama:\n");
			USEAGE();
			return -1;
		}
		opt->use_arg = 1;
		opt->value = (char*)malloc(strlen(optarg) + 1);
		strcpy(opt->value, optarg);
	}  
	if(get_app_option_by_name('a')->use_arg ||
		get_app_option_by_name('A')->use_arg ||
		get_app_option_by_name('p')->use_arg ||
		get_app_option_by_name('P')->use_arg)
		useSelect = true;
	opt = get_app_option_by_name('f');
	if(!opt->value)
	{
		USEAGE();
		return -1;
	}
	strcpy(filename, opt->value);
	//2.0 打开文件并解析
	pcap = PcapFile::Open(filename);
	ParsePacpFile(pcap);
	PacketArray* arr = pcap->GetPacketArray();
	//3.0 按参数配置进行包筛选
	if(useSelect)
		arr = arr->Select(selector);
#ifdef DEBUG
	for(int i = 0; i < arr->GetCount(); i++)
	{
		printf("***********************%d***************************\n", i);
		arr->GetPacketByIndex(i)->PrintInfo();
	}
#endif
	//4.0 输出结果
	opt = get_app_option_by_name('c');
	if(!opt->value || strcmp(strlwr(opt->value), "all") == 0)
	{
		printf("packet_count:%d\n", arr->GetCount());
		printf("max packet:%d\n",GetMaxSize(arr));
		printf("bandwidth:%0.3f Kbps\n", ComputeBandwidth(arr));
	}
	else if(strcmp(strlwr(opt->value), "get_max_packet_size") == 0)
		printf("max packet:%d\n",GetMaxSize(arr));
	else if(strcmp(strlwr(opt->value), "count_packet") == 0)
		printf("packet_count:%d\n", arr->GetCount());
	else if(strcmp(strlwr(opt->value), "compute_bandwidth") == 0)
		printf("bandwidth:%0.3f Kbps\n", ComputeBandwidth(arr));
	else 
	{
		printf("error function command! \n");
		USEAGE();
	}
	//5.0 释放主动申请的内存
	if(useSelect)
		delete arr;
	delete pcap;
	free_options_value();
	return 0;
}

