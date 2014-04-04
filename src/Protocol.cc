#include "Protocol.h"
#include <stdio.h>
#include "Packet.h"
#include "EtherNet.h"
Protocol::Protocol(Protocol* obj)
:eProto(UNKOWN), uiOffset(0), uiHeaderLen(0), pstPreProtcol(obj), pstPostProtcol(NULL), pstBuffer(NULL), pstPayload(NULL)
{
	pstPacket = obj->pstPacket;
}

Protocol::Protocol(Packet& pack)
: eProto(UNKOWN), uiOffset(0), uiHeaderLen(0), pstPreProtcol(NULL), pstPostProtcol(NULL), pstBuffer(NULL), pstPayload(NULL)
{
	pstPacket = &pack;
}

uint32 Protocol::GetOffSet()
{
	return uiOffset;
}
uint32 Protocol::GetHeaderLen()
{
	return uiHeaderLen;
}
Buffer* Protocol::GetBuffer()
{
	return pstBuffer;
}
Buffer* Protocol::GetPayload()
{
	return pstPayload;
}
bool Protocol::CheckBuff()
{
	if(!pstPacket || !(pstPacket->GetData()))
		return false;
	pstBuffer = pstPacket->GetData()->GetBufferByOffSet(uiOffset);
	if(!pstBuffer)
		return false;
	pstPayload = pstBuffer->GetBufferByOffSet(uiHeaderLen);
	if(!pstPayload)
		return false;
	return true;
}
//解析上层协议，构建协议栈
bool Protocol::Parse()
{
	return CheckBuff();
}
Protocol* Protocol::GetUpperProtocol()
{
	if(pstPostProtcol == NULL)
	{
		pstPostProtcol = new EtherNet(this);
		pstPostProtcol->SetOffSet(uiOffset + uiHeaderLen);
	}
	return pstPostProtcol;
}
Protocol* Protocol::GetLowerProtocol()
{
	return pstPreProtcol;
}
void Protocol::SetLowerProtocol(Protocol* pre)
{
	pstPreProtcol = pre;
}
proto_type Protocol::GetType()
{
	return eProto;
}
void Protocol::SetOffSet(uint32 offset)
{
	uiOffset = offset;
}

void Protocol::PrintInfo()
{}
Protocol::~Protocol(void)
{
	if (pstBuffer)
	{
		delete pstBuffer;
		pstBuffer = NULL;
	}	
	if (pstPayload)
	{
		delete pstPayload;
		pstPayload = NULL;
	}
		
}
