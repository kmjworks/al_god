#include "logger.h"
#include <errno.h>
#include <sys/stat.h>

// Global logger instance
Logger *g_logger = NULL;

// Log level names
static const char *log_level_names[LOG_LEVEL_COUNT] = {
    "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};

// Log level colors
static const char *log_level_colors[LOG_LEVEL_COUNT] = {
    COLOR_TRACE, COLOR_DEBUG, COLOR_INFO, 
    COLOR_WARN, COLOR_ERROR, COLOR_FATAL
};

// Initialize mutex based on platform
static void mutex_init(Logger *logger) {
#ifdef PLATFORM_WINDOWS
    InitializeCriticalSection(&logger->mutex);
#else
    pthread_mutex_init(&logger->mutex, NULL);
#endif
}

// Lock mutex
static void mutex_lock(Logger *logger) {
#ifdef PLATFORM_WINDOWS
    EnterCriticalSection(&logger->mutex);
#else
    pthread_mutex_lock(&logger->mutex);
#endif
}

// Unlock mutex
static void mutex_unlock(Logger *logger) {
#ifdef PLATFORM_WINDOWS
    LeaveCriticalSection(&logger->mutex);
#else
    pthread_mutex_unlock(&logger->mutex);
#endif
}

// Destroy mutex
static void mutex_destroy(Logger *logger) {
#ifdef PLATFORM_WINDOWS
    DeleteCriticalSection(&logger->mutex);
#else
    pthread_mutex_destroy(&logger->mutex);
#endif
}

// Get file size
static size_t get_file_size(FILE *file) {
    if (!file) return 0;
    
    long current_pos = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, current_pos, SEEK_SET);
    
    return size;
}

// Create logger
Logger* logger_create(const LogConfig *config) {
    Logger *logger = (Logger*)calloc(1, sizeof(Logger));
    if (!logger) {
        fprintf(stderr, "Failed to allocate logger\n");
        return NULL;
    }
    
    // Copy configuration
    logger->config = *config;
    
    // Initialize mutex
    mutex_init(logger);
    
    // Open log file if needed
    if (logger->config.log_to_file && strlen(logger->config.log_file_path) > 0) {
        logger->file = fopen(logger->config.log_file_path, "a");
        if (!logger->file) {
            fprintf(stderr, "Failed to open log file: %s\n", 
                    logger->config.log_file_path);
            free(logger);
            return NULL;
        }
        logger->current_file_size = get_file_size(logger->file);
    }
    
    return logger;
}

// Destroy logger
void logger_destroy(Logger *logger) {
    if (!logger) return;
    
    mutex_lock(logger);
    
    if (logger->file) {
        fclose(logger->file);
        logger->file = NULL;
    }
    
    mutex_unlock(logger);
    mutex_destroy(logger);
    
    free(logger);
}

// Initialize default logger
void logger_init_default(void) {
    LogConfig config = {
        .min_level = LOG_INFO,
        .use_colors = true,
        .log_to_file = false,
        .log_to_console = true,
        .include_timestamp = true,
        .include_file_info = true,
        .log_file_path = "",
        .max_file_size = 10 * 1024 * 1024,  // 10MB
        .max_backup_files = 5
    };
    
    g_logger = logger_create(&config);
}

// Rotate log file
void logger_rotate_file(Logger *logger) {
    if (!logger || !logger->file) return;
    
    mutex_lock(logger);
    
    // Close current file
    fclose(logger->file);
    
    // Rotate backup files
    for (int i = logger->config.max_backup_files - 1; i > 0; i--) {
        char old_name[512], new_name[512];
        
        if (i == 1) {
            snprintf(old_name, sizeof(old_name), "%s", 
                     logger->config.log_file_path);
        } else {
            snprintf(old_name, sizeof(old_name), "%s.%d", 
                     logger->config.log_file_path, i - 1);
        }
        
        snprintf(new_name, sizeof(new_name), "%s.%d", 
                 logger->config.log_file_path, i);
        
        rename(old_name, new_name);
    }
    
    // Open new file
    logger->file = fopen(logger->config.log_file_path, "w");
    logger->current_file_size = 0;
    
    mutex_unlock(logger);
}

// Main logging function
void logger_log(Logger *logger, LogLevel level, const char *file, 
                int line, const char *format, ...) {
    if (!logger || level < logger->config.min_level) return;
    
    mutex_lock(logger);
    
    // Get current time
    time_t raw_time = time(NULL);
    struct tm *time_info = localtime(&raw_time);
    char time_str[32];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    
    // Extract filename from path
    const char *filename = file;
    const char *last_sep = strrchr(file, '/');
    if (!last_sep) last_sep = strrchr(file, '\\');
    if (last_sep) filename = last_sep + 1;
    
    // Format message
    char message[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(message, sizeof(message), format, args);
    va_end(args);
    
    // Prepare full log line
    char full_message[2048];
    int written = 0;
    
    if (logger->config.include_timestamp) {
        written += snprintf(full_message + written, 
                           sizeof(full_message) - written,
                           "[%s] ", time_str);
    }
    
    if (logger->config.use_colors && logger->config.log_to_console) {
        written += snprintf(full_message + written, 
                           sizeof(full_message) - written,
                           "%s[%-5s]%s ", 
                           log_level_colors[level], 
                           log_level_names[level], 
                           COLOR_RESET);
    } else {
        written += snprintf(full_message + written, 
                           sizeof(full_message) - written,
                           "[%-5s] ", log_level_names[level]);
    }
    
    if (logger->config.include_file_info) {
        written += snprintf(full_message + written, 
                           sizeof(full_message) - written,
                           "%s:%d: ", filename, line);
    }
    
    written += snprintf(full_message + written, 
                       sizeof(full_message) - written,
                       "%s\n", message);
    
    // Output to console
    if (logger->config.log_to_console) {
        FILE *output = (level >= LOG_ERROR) ? stderr : stdout;
        fprintf(output, "%s", full_message);
        fflush(output);
    }
    
    // Output to file
    if (logger->file) {
        // Remove color codes for file output
        if (logger->config.use_colors) {
            char clean_message[2048];
            int j = 0;
            bool in_escape = false;
            
            for (int i = 0; full_message[i] && j < sizeof(clean_message) - 1; i++) {
                if (full_message[i] == '\x1b') {
                    in_escape = true;
                } else if (in_escape && full_message[i] == 'm') {
                    in_escape = false;
                } else if (!in_escape) {
                    clean_message[j++] = full_message[i];
                }
            }
            clean_message[j] = '\0';
            
            fprintf(logger->file, "%s", clean_message);
            logger->current_file_size += strlen(clean_message);
        } else {
            fprintf(logger->file, "%s", full_message);
            logger->current_file_size += written;
        }
        
        fflush(logger->file);
        
        // Check if rotation is needed
        if (logger->config.max_file_size > 0 && 
            logger->current_file_size >= logger->config.max_file_size) {
            logger_rotate_file(logger);
        }
    }
    
    mutex_unlock(logger);
}

// Set minimum log level
void logger_set_level(Logger *logger, LogLevel level) {
    if (!logger) return;
    mutex_lock(logger);
    logger->config.min_level = level;
    mutex_unlock(logger);
}

// Enable/disable colors
void logger_enable_colors(Logger *logger, bool enable) {
    if (!logger) return;
    mutex_lock(logger);
    logger->config.use_colors = enable;
    mutex_unlock(logger);
}