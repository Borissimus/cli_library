#include "log.h"
#include <stdio.h>
#include <stdarg.h>

// Define the log level prefix strings
static const char *log_level_prefix[] = {
	"INFO ",
	"DEBUG ",
	"WARNING ",
	"ERROR ",
	""
};

// Logging functions for each level
static void log_message(const char *format, ...);
static void log_debug(const char *format, ...);
static void log_info(const char *format, ...);
static void log_warn(const char *format, ...);
static void log_error(const char *format, ...);

static console_api_t console_api = {
	.info  = log_info,
	.debug = log_debug,
	.warn  = log_warn,
	.error = log_error,
	.print = log_message
};

// Internal function to write the log message to the appropriate output
static void log_write(log_level_t level, const char *format, va_list args) {
	printf("%s\n\r", log_level_prefix[level]);
}

// Logging functions for each level
static void log_message(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_write(LOG_LEVEL_MESSAGE, format, args);
    va_end(args);
}

static void log_debug(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_write(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

static void log_info(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_write(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

static void log_warn(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_write(LOG_LEVEL_WARN, format, args);
    va_end(args);
}

static void log_error(const char *format, ...) {
    va_list args;
    va_start(args, format);
    log_write(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

// Get the console_api instance
console_api_t* get_console(void) {
	return &console_api;
}

// Initialize the console instance
void init_console_instance(console_api_t* console) {
	printf("Initializing console instance\n\r");
	console_api_t* console_api = get_console();
	console->info = console_api->info;
	console->debug = console_api->debug;
	console->warn = console_api->warn;
	console->error = console_api->error;
	console->print = console_api->print;
}