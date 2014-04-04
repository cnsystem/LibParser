#include "PacketArray.h"


PacketArray::PacketArray(void): count(0),head(NULL),tail(NULL)
{
}

PacketArray::PacketArray(Packet* list): count(0),head(NULL),tail(NULL)
{
	SetPacketList(list);
}
PacketArray* PacketArray::Select(SelectPacket func)
{
	PacketArray* pArray = new PacketArray();
	Packet* temp = head;
	while(temp != NULL)
	{
		if(func(temp))
		{
			Packet* packet = new Packet(temp->GetBuffer());
			packet->Parse();
			pArray->AddPacket(packet);
		}
		temp = temp->next;
	}
	return pArray;
}
void PacketArray::SetPacketList(Packet* list)
{
	Packet* temp = list;
	head = temp;
	while(temp != NULL)
	{
		temp = list;
		count++;
		list = list->next;
	}
	tail = temp;
}
void PacketArray::AddPacket(Packet* pack)
{
	if(tail != NULL)
	{
		pack->pre = tail;
		pack->next = tail->next;
		tail->next = pack;
	}
	else
	{
		head = pack;
	}
	tail = pack;
	count++;
}
Packet* PacketArray::GetPacketByIndex(uint32 index)
{
	Packet* pack = head;
	if(head == NULL || index > count - 1)
		return NULL;
	for (uint32 i = 0; i< index && pack; i++)
		pack = pack->next;
	return pack;
}
bool PacketArray::IsEmpty()
{
	if(count == 0)
		return true;
	return false;
}
uint32 PacketArray::GetCount()
{
	return count;
}
PacketArray::~PacketArray(void)
{
	Packet* temp = head;
	while(temp != NULL)
	{
		Packet* p = temp;
		temp = temp->next;
		count--;
		delete p;
	}
	head = NULL;
	tail = NULL;
}
