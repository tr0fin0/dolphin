#include <stdio.h>
#include <stdarg.h>
#include "logging.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"

static MessageBufferHandle_t message_buffer = NULL; /** Cross-core message buffer. */

/**
 * @brief Dedicated logging task on Core 0.
 */
static void vLogTask(void *pvParameters) {
    char rx_buffer[LOG_BUFFER_SIZE];

    while (1) {
        size_t received_bytes = xMessageBufferReceive(
            message_buffer,
            rx_buffer,
            sizeof(rx_buffer),
            portMAX_DELAY
        );

        if (received_bytes > 0) {
            printf("%.*s", (int) received_bytes, rx_buffer);
        }
    }
}

/**
 * @brief Asynchronous logging vprintf based function.
 */
static int async_log_vprintf(const char *fmt, va_list args) {
    if (message_buffer == NULL) {
        return 0;
    }

    char buffer[LOG_BUFFER_SIZE];
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);

    if (len > 0 && len < sizeof(buffer)) {
        xMessageBufferSend(message_buffer, buffer, len, 0);
    }

    return len;
}

void log_init_async(void) {
    message_buffer = xMessageBufferCreate(LOG_MESSAGE_SIZE);

    if (message_buffer != NULL) {
        xTaskCreatePinnedToCore(
            vLogTask,
            "LogTask",
            LOG_MESSAGE_SIZE,
            NULL,
            1,
            NULL,
            0
        );

        esp_log_set_vprintf(async_log_vprintf);
    }
}
