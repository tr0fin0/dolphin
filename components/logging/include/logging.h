/**
 * @file logging.h
 * @brief Logging macro wrappers of `esp_log.h` macro functions.
 *
 * @author Guilherme Nunes Trofino
 * @date 2026-04-24
 */

#pragma once

#include "esp_log.h"

#define LOG_BUFFER_SIZE     256
#define LOG_MESSAGE_SIZE    4096

#if (LOG_DEFAULT_LEVEL <= LOG_DEFAULT_LEVEL_ERROR)
/**
 * @brief Log message with error severity.
 */
#define LOG_E(msg, ...) \
    ESP_LOGE(__FILENAME__, "%s() " msg, __func__, ##__VA_ARGS__)
#else
#define LOG_E(msg, ...) {}
#endif

#if (LOG_DEFAULT_LEVEL <= LOG_DEFAULT_LEVEL_WARN)
/**
 * @brief Log message with warning severity.
 */
#define LOG_W(msg, ...) \
    ESP_LOGW(__FILENAME__, "%s() " msg, __func__, ##__VA_ARGS__)
#else
#define LOG_W(msg, ...) {}
#endif

#if (LOG_DEFAULT_LEVEL <= LOG_DEFAULT_LEVEL_INFO)
/**
 * @brief Log message with info severity.
 */
#define LOG_I(msg, ...) \
    ESP_LOGI(__FILENAME__, "%s() " msg, __func__, ##__VA_ARGS__)
#else
#define LOG_I(msg, ...) {}
#endif

#if (LOG_DEFAULT_LEVEL <= LOG_DEFAULT_LEVEL_DEBUG)
/**
 * @brief Log message with debug severity.
 */
#define LOG_D(msg, ...) \
    ESP_LOGD(__FILENAME__, "%s() " msg, __func__, ##__VA_ARGS__)
#else
#define LOG_D(msg, ...) {}
#endif

#if (LOG_DEFAULT_LEVEL <= LOG_DEFAULT_LEVEL_VERBOSE)
/**
 * @brief Log message with verbose severity.
 */
#define LOG_V(msg, ...) \
    ESP_LOGV(__FILENAME__, "%s() " msg, __func__, ##__VA_ARGS__)
#else
#define LOG_V(msg, ...) {}
#endif

/**
 * @brief Initializes asynchronous dual-core logging.
 *
 * Routes all logging calls through a message buffer to a dedicated task on
 * Core 0.
 */
void log_init_async(void);
