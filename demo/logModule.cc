#include "logModule.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
char* file_list[2] = {"external.log","main.log"};
LOG_LEVEL global_loglevel = LOG_DEBUG;

char* basename(const char* file_path)
{
	char* filename = NULL;	
	filename = (char*)strrchr(file_path, '\\');
	if(NULL == filename)
		return (char*)file_path;
	return ++filename;
}

void log_print(char* filename, LOG_LEVEL level, const char* file, long line, const char* format, ...)
{
	#define BUFFER_LEN (400)
	va_list va;
	int str_len;
	char output_buf[BUFFER_LEN];
	char file_path[256];
	FILE* log_file = stdout;
	
	log_file = fopen(filename, "a+");
	if( level >= global_loglevel)
	{
		str_len = _snprintf(output_buf,  BUFFER_LEN, "[%-10s:%2d]\t",  basename(file), line);
		va_start(va, format);
		vsnprintf(output_buf + str_len, BUFFER_LEN - str_len, format, va);
	}
	fclose(log_file);
}

void set_log_level(LOG_LEVEL level)
{
	global_loglevel = level;
}
