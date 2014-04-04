#ifndef LOG_MODULE_H
#define LOG_MODULE_H
typedef enum
{
	LOG_ERROR = 0,
	LOG_WARNING,
	LOG_INFO,
	LOG_DEBUG
}LOG_LEVEL;
typedef enum 
{
	External = 0,
	Main = 1	
}Module;
#define LOG_OUTPUT(module,level,...) log_print(file_list[module],level,__FILE__,__LINE,__VA_ARGS__)

char* basename(const char* file_path);

void log_print(char* filename, LOG_LEVEL level, const char* file, long line, const char* format, ...);

void set_log_level(LOG_LEVEL level);

#endif