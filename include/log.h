#ifndef LOG_H
#define LOG_H

typedef enum {
    LOG_LEVEL_INFO = 0,
	LOG_LEVEL_DEBUG,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
	LOG_LEVEL_MESSAGE
} log_level_t;

typedef struct {
	void (*info)(const char *format, ...);
	void (*debug)(const char *format, ...);
	void (*warn)(const char *format, ...);
	void (*error)(const char *format, ...);
	void (*print)(const char *format, ...);
} console_api_t;

console_api_t* get_console(void);

void init_console_instance(console_api_t* console);

#endif // LOG_H
