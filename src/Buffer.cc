#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Buffer::Buffer(uint32 length)
{
	pstBuffer = (byte*)malloc(length);
	uiLen = length;
	bCreate = true;
}
Buffer::Buffer(byte* buffer, uint32 len)
{
	pstBuffer = buffer;
	uiLen = len;
	bCreate = false;
}

void Buffer::ResetLen(uint32 len)
{
	if(len <= uiLen)
		return;
	byte* buf = (byte*)malloc(len);
	memset(buf, 0, len);
	memcpy(buf, pstBuffer, uiLen);
	if(bCreate)
		free(pstBuffer);
	pstBuffer = buf;
	uiLen = len;
	bCreate = true;
}
byte* Buffer::GetBuffer(uint32* length)
{
	*length = uiLen;
	return pstBuffer;
}

Buffer* Buffer::GetBufferByOffSet(uint32 offset)
{
	Buffer* newBuf = NULL;
	byte* buf = pstBuffer + offset;
	uint32 len = uiLen - offset;
	if(len >= 0)
		newBuf = new Buffer(buf, len);
	return newBuf;
}
Buffer* Buffer::GetBufferByOffSet(uint32 start, uint32 end)
{
	Buffer* newBuf = NULL;
	byte* buf = pstBuffer + start;
	uint32 len = uiLen - start - end;
	if(len >= 0)
		newBuf = new Buffer(buf, len);
	return newBuf;
}
Buffer::~Buffer(void)
{
	if(bCreate)
		free(pstBuffer);
}
