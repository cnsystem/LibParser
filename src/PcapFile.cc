#include <stdio.h>
#include <stdlib.h>
#include "Packet.h"
#include "Buffer.h"
#include "PcapFile.h"
#define CHECKANDRETURN(cond, value) if((cond)) return (value);

void PrintFileHead(pcap_file_header* head)
{
	printf("  head->magic: %d \n", head->magic);
	printf("  head->version_major: %d \n", head->version_major);
	printf("  head->version_minor: %d \n", head->version_minor);
	printf("  head->thiszone: %d \n", head->thiszone);
	printf("  head->sigfigs: %d \n", head->sigfigs);
	printf("  head->snaplen: %d \n", head->snaplen);
	printf("  head->linktype: %d \n", head->linktype);
}

void PrintPacketHead(packet_hdr* head)
{
	printf("  head->ts.tv_sec: %d \n", head->ts.tv_sec);
	printf("  head->ts.tv_sec: %d \n", head->ts.tv_usec);
	printf("  head->caplen: %d \n", head->caplen);
	printf("  head->len: %d \n", head->len);
}
PcapFile::PcapFile(void):file(NULL), buffer(NULL)
{
	packList = new PacketArray();
}

PcapFile* PcapFile::Open(char* filename)
{
	PcapFile* pcap = new PcapFile();
	fopen_s(&pcap->file, filename, "rb");
	return pcap;
}
uint32 PcapFile::GetPacketCount()
{
	return packList->GetCount();
}
int PcapFile::ReadFile()
{
	int len = 0;
	CHECKANDRETURN(file == NULL, -1);
	len = fread(&header, 1, sizeof(header), file);
	//CHECKANDRETURN(len != sizeof(header), -1);
	CHECKANDRETURN(header.magic != 0xa1b2c3d4, -1);
	fseek(file, 0, SEEK_END);
	length = ftell(file) - sizeof(header);
	CHECKANDRETURN(length <= 0, -1);
	fseek(file, len, SEEK_SET);
	buffer = (uint8*)malloc(length);
	CHECKANDRETURN(buffer == NULL, -1);
	len = fread(buffer, 1, length, file);
	CHECKANDRETURN(len != length, -1);
	fclose(file);
	file = NULL;
	PrintFileHead(&header);
	return 0;
}
int PcapFile::Parse()
{
	uint32 len = (uint32)length;
	uint8* temp = NULL;
	int ret = 0;
	temp = buffer;
	while(len > sizeof(packet_hdr))
	{
		packet_hdr* hdr = (packet_hdr*)temp;
		len -= sizeof(packet_hdr);		
		CHECKANDRETURN(len < hdr->len, -1);
		Buffer* packet_buf = new Buffer(temp, hdr->len + sizeof(packet_hdr));
		Packet* packet = new Packet(packet_buf);
		packList->AddPacket(packet);
		len -= hdr->len;
		temp += hdr->len + sizeof(packet_hdr);
		printf("packet index: %d\n", packList->GetCount());
		PrintPacketHead(hdr);
	}
	return 0;
}
PacketArray* PcapFile::GetPacketArray()
{
	return packList;
}
Packet* PcapFile::GetPacket()
{
	return packList->GetPacketByIndex(0);
}
PcapFile::~PcapFile(void)
{
	if(file)
	{
		fclose(file);
		file = NULL;
	}
	if(buffer)
	{
		free(buffer);
		file = NULL;
	}
	if(packList)
	{
		delete packList;
		packList = NULL;
	}
}
