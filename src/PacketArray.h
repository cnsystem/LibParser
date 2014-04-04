#pragma once
#include "Packet.h"
typedef bool (*SelectPacket)(Packet*);
class PacketArray
{
public:
	PacketArray();
	PacketArray(Packet* list);
	PacketArray* Select(SelectPacket func);
	void SetPacketList(Packet* list);
	void AddPacket(Packet* pack);
	bool IsEmpty();
	uint32 GetCount();
	Packet* GetPacketByIndex(uint32 index);
	~PacketArray(void);
protected:
	uint32 count;
	Packet* tail;
	Packet* head;
};

