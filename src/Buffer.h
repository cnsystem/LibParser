#pragma once
#include "types.h"
#include <stdio.h>
class Buffer
{
public:
	Buffer(void):pstBuffer(NULL),uiLen(0),bCreate(false){};
	Buffer(uint32 length);
	Buffer(byte* buffer, uint32 len);
	~Buffer(void);

	void ResetLen(uint32 len);
	byte* GetBuffer(uint32* length);
	Buffer* GetBufferByOffSet(uint32 offset);
	Buffer* GetBufferByOffSet(uint32 start, uint32 end);
private:
	byte*	pstBuffer;
	uint32	uiLen;
	bool bCreate;
};

