#pragma once
#include "types.h"
class StringHelper
{
public:
	static void byte2str(byte data, int8& ch0, int8& ch1);
	static int32 int2str(uint32 data, int8* buffer);
	static void reverse(int8* buffer, int32 start, int32 end);
	static void reverse(int8* buffer, int32 length);
private:
	StringHelper(void);
	~StringHelper(void);
};

