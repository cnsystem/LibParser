#include "StringHelper.h"


StringHelper::StringHelper(void)
{
}

void StringHelper::byte2str(byte data, int8& ch0, int8& ch1)
{
	ch0 = (data>>4) & 0x0F;
	ch0 = ch0 > 9 ? ch0 - 10 + 'A' : ch0 + '0';
	ch1 = data & 0x0F;
	ch1 = ch1 > 9 ? ch1 - 10 + 'A' : ch1 + '0';
}
int32 StringHelper::int2str(uint32 data, int8* buffer)
{
	int32 index = 0;
	//此处先倒序存储
	do
	{
		buffer[index] = data % 10 + '0'; 
		index++;
	}while((data = data/10) != 0);
	//此处在倒序一下
	reverse(buffer, index);
	return index;
}
void StringHelper::reverse(int8* buffer, int32 start, int32 end)
{
	int middle = (end - start)/2;
	int8 temp;
	for(int i = start; i <= middle; i++)
	{
		temp = buffer[i];
		buffer[i] = buffer[end - i];
		buffer[end - i] = temp;
	}
}
void StringHelper::reverse(int8* buffer, int32 length)
{
	reverse(buffer, 0, length - 1);
}
StringHelper::~StringHelper(void)
{
}
