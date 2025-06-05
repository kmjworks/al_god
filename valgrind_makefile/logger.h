#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #define PLATFORM_WINDOWS
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
#else
    #define PLATFORM_UNIX
    #include <unistd.h>
    #include <pthread.h>
    #define PATH_SEPARATOR "/"
#endif

// Log levels
typedef enum {
    LOG_TRACE = 0,
    LOG_DEBUG = 1,
    LOG_INFO = 2,
    LOG_WARN = 3,
    LOG_ERROR = 4,
    LOG_FATAL = 5,
    LOG_LEVEL_COUNT
} LogLevel;

// ANSI color codes
#ifdef PLATFORM_UNIX
    #define COLOR_TRACE   "\x1b[37m"    // White
    #define COLOR_DEBUG   "\x1b[36m"    // Cyan
    #define COLOR_INFO    "\x1b[32m"    // Green
    #define COLOR_WARN    "\x1b[33m"    // Yellow
    #define COLOR_ERROR   "\x1b[31m"    // Red
    #define COLOR_FATAL   "\x1b[35m"    // Magenta
    #define COLOR_RESET   "\x1b[0m"
#else
    #define COLOR_TRACE   ""
    #define COLOR_DEBUG   ""
    #define COLOR_INFO    ""
    #define COLOR_WARN    ""
    #define COLOR_ERROR   ""
    #define COLOR_FATAL   ""
    #define COLOR_RESET   ""
#endif

// Logger configuration
typedef struct {
    LogLevel min_level;
    bool use_colors;
    bool log_to_file;
    bool log_to_console;
    bool include_timestamp;
    bool include_file_info;
    char log_file_path[256];
    size_t max_file_size;
    int max_backup_files;
} LogConfig;

// Logger structure
typedef struct {
    LogConfig config;
    FILE *file;
    size_t current_file_size;
#ifdef PLATFORM_WINDOWS
    CRITICAL_SECTION mutex;
#else
    pthread_mutex_t mutex;
#endif
} Logger;

// Global logger instance
extern Logger *g_logger;

// Logger lifecycle functions
Logger* logger_create(const LogConfig *config);
void logger_destroy(Logger *logger);
void logger_init_default(void);

// Logging functions
void logger_log(Logger *logger, LogLevel level, const char *file, 
                int line, const char *format, ...);
void logger_set_level(Logger *logger, LogLevel level);
void logger_enable_colors(Logger *logger, bool enable);
void logger_rotate_file(Logger *logger);

// Convenience macros
#define LOG_TRACE(...) \
    if (g_logger) logger_log(g_logger, LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_DEBUG(...) \
    if (g_logger) logger_log(g_logger, LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) \
    if (g_logger) logger_log(g_logger, LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...) \
    if (g_logger) logger_log(g_logger, LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) \
    if (g_logger) logger_log(g_logger, LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) \
    if (g_logger) logger_log(g_logger, LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

// Assert with logging
#define LOG_ASSERT(condition, ...) \
    do { \
        if (!(condition)) { \
            LOG_FATAL("Assertion failed: " #condition); \
            LOG_FATAL(__VA_ARGS__); \
            abort(); \
        } \
    } while(0)

// Performance logging
#define LOG_TIMER_START(name) \
    clock_t _timer_##name = clock()

#define LOG_TIMER_END(name) \
    do { \
        double _elapsed = ((double)(clock() - _timer_##name)) / CLOCKS_PER_SEC; \
        LOG_DEBUG("Timer [" #name "] elapsed: %.3f seconds", _elapsed); \
    } while(0)

#endif // LOGGER_H