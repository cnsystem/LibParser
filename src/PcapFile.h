#pragma once
#include <stdio.h>
#include "types.h"
#include "PacketArray.h"
typedef struct pcap_file_header
{
	uint32 magic; //��ʶλ��32λ�ģ������ʶλ��ֵ��16���Ƶ� 0xa1b2c3d4��
	uint16 version_major; //���汾�ţ�16λ�� Ĭ��ֵΪ0x2��
	uint16 version_minor; //���汾�ţ�16λ��Ĭ��ֵΪ0x04��
	uint32 thiszone; //����ʱ�䣺32λ��ʵ���ϸ�ֵ��δʹ�ã���˿��Խ���λ����Ϊ0��
	uint32 sigfigs; //��ȷʱ�����32λ��ʵ���ϸ�ֵ��δʹ�ã���˿��Խ���ֵ����Ϊ0��
	uint32 snaplen; //���ݰ���󳤶ȣ�32λ����ֵ������ץ������ݰ�����󳤶�
	uint32 linktype; //��·�����ͣ�32λ�� ���ݰ�����·���ͷ��������·�������
}pcap_file_header;
/*************************************************************************
//����������ֵ����·�����͵Ķ�Ӧ��
//0          BSD       loopback devices, except for later OpenBSD
//1          Ethernet, and Linux loopback devices   ��̫�����ͣ�����������ݰ�Ϊ�������͡�
//6          802.5 Token Ring
//7          ARCnet
//8          SLIP
//9          PPP
//10         FDDI
//100        LLC/SNAP-encapsulated ATM
//101        raw IP, with no link
//102        BSD/OS SLIP
//103        BSD/OS PPP
//104        Cisco HDLC
//105        802.11
//108        later OpenBSD loopback devices (with the AF_value in network byte order)
//113        special Linux cooked capture
//114        LocalTalk
*************************************************************************/


class PcapFile
{
public:
	static PcapFile* Open(char* filename);	
	int ReadFile(void);
	int Parse(void);
	uint32 GetPacketCount();
	Packet* GetPacket();
	PacketArray* GetPacketArray();
	~PcapFile(void);
protected:
	PacketArray *packList;
private:
	PcapFile(void);
	FILE* file;
	pcap_file_header header;
	int length;
	uint8* buffer;
};

