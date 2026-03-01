#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif



#define BUFFER_SIZE_DATETIME    24
#define BUFFER_SIZE_HEADER      128
#define BUFFER_SIZE_MESSAGE     384
#define BUFFER_SIZE_PRINT       (BUFFER_SIZE_HEADER + BUFFER_SIZE_MESSAGE + 32)

#define MESSAGE_COLOR_RESET     "\033[0m"
#define MESSAGE_COLOR_RED       "\033[31m"
#define MESSAGE_COLOR_GREEN     "\033[32m"
#define MESSAGE_COLOR_YELLOW    "\033[33m"
#define MESSAGE_COLOR_MAGENTA   "\033[35m"
#define MESSAGE_COLOR_CYAN      "\033[36m"


/**
 * @brief Defines debug logging levels.
 * 
 * Available levels are:
 * 
 * - `DEBUG_LEVEL_CRITICAL`:    System Failure
 *
 * - `DEBUG_LEVEL_ERROR`:       Recoverable Error
 *
 * - `DEBUG_LEVEL_WARNING`:     Warning Condition
 *
 * - `DEBUG_LEVEL_INFO`:        Informational Message
 *
 * - `DEBUG_LEVEL_TRACE`:       Trace-level Diagnostic Message
 */
typedef enum {
    /**
     * @brief System Failure.
     *
     * Unrecoverable errors that may cause the system to stop functioning
     * properly.
     */
    DEBUG_LEVEL_CRITICAL = 1,

    /**
     * @brief Recoverable Error.
     *
     * Indicates errors that occurred but do not prevent the system from
     * continuing operation.
     */
    DEBUG_LEVEL_ERROR    = 2,

    /**
     * @brief Warning Condition.
     *
     * Indicates unexpected behavior or potential issues that do not necessarily
     * represent an error.
     */
    DEBUG_LEVEL_WARNING  = 3,

    /**
     * @brief Informational Message.
     *
     * Indicates normal operational messages such as state transitions or
     * configuration updates.
     */
    DEBUG_LEVEL_INFO     = 4,

    /**
     * @brief Trace-level Diagnostic Message.
     *
     * Provides detailed debugging information such as raw data values and
     * execution flow tracking.
     */
    DEBUG_LEVEL_TRACE    = 5
} debug_level_t;



/**
 * @brief Initialize the debug serial port.
 */
void debug_init(uint32_t baud_rate);

/**
 * @brief Serial Print formatted message with contextual data.
 *
 * Contextual This function prints a formatted debug message to the configured
 * output interface (e.g., Serial), including:
 *  - Timestamp
 *  - Debug level
 *  - Source file
 *  - Function name
 *  - Line number
 *
 * It is intended to be called through the DEBUG_MSG macro so that
 * file, function, and line information are automatically captured
 * at the call site.
 *
 * @param level Debug severity level (see debug_level_t).
 * @param file  Source file name where the log was generated.
 * @param func  Function name where the log was generated.
 * @param line  Line number where the log was generated.
 * @param fmt   printf-style format string.
 * @param ...   Additional arguments corresponding to the format string.
 *
 * @note `printf`-style formatting support.
 * @note Messages with level higher than `SYSTEM_DEBUG_LEVEL` are ignored.
 *
 * @warning Avoid calling this function directly; use DEBUG_MSG instead.
 */
void debug_message(int level, const char *file, const char *func, int line, const char *fmt, ...);


#ifndef DEBUG_MSG
/**
 * @brief Serial Print formatted message with contextual data.
 *
 * Include contextual data:
 *
 * - `__FILE__`
 *
 * - `__func__`
 *
 * - `__LINE__`
 *
 * @param level `debug_level_t` bebug severity level.
 * @param fmt   `string` printf-style message.
 * @param ...   Additional arguments corresponding to the format string.
 */
#define DEBUG_MSG(level, fmt, ...) \
    debug_message((level), (__FILE__), (__func__), (__LINE__), (fmt), ##__VA_ARGS__)
#endif



#ifdef __cplusplus
}
#endif

#endif /* __DEBUG_H__ */
