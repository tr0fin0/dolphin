#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/message_buffer.h"
#include "logging.h"
#include <stdarg.h>
#include <stdio.h>

/**
 * @brief Cross-core message buffer.
 */
static MessageBufferHandle_t logging_buffer_handle = NULL;

/**
 * @brief Dedicated logging task on Core 0.
 */
static void vLogTask(void *pvParameters) {
    char received_buffer[LOGGING_BUFFER_SIZE];

    while (1) {
        size_t received_buffer_size = xMessageBufferReceive(
            logging_buffer_handle,
            received_buffer,
            sizeof(received_buffer),
            portMAX_DELAY
        );

        if (received_buffer_size > 0) {
            printf("%.*s", (int) received_buffer_size, received_buffer);
        }
    }
}

/**
 * @brief Asynchronous logging vprintf based function.
 */
static int logging_vprintf_async(const char *fmt, va_list args) {
    if (logging_buffer_handle == NULL) {
        return 0;
    }

    char transmit_buffer[LOGGING_BUFFER_SIZE];
    int transmit_message_size = vsnprintf(
        transmit_buffer,
        sizeof(transmit_buffer),
        fmt,
        args
    );

    if (
        transmit_message_size > 0 &&
        transmit_message_size < sizeof(transmit_buffer)
    ) {
        xMessageBufferSend(
            logging_buffer_handle,
            transmit_buffer,
            transmit_message_size,
            0
        );
    }

    return transmit_message_size;
}

void logging_init_async(void) {
    logging_buffer_handle = xMessageBufferCreate(LOGGING_MESSAGE_SIZE);

    if (logging_buffer_handle != NULL) {
        xTaskCreatePinnedToCore(
            vLogTask,
            "LogTask",
            LOGGING_MESSAGE_SIZE,
            NULL,
            LOGGING_TASK_PRIORITY,
            NULL,
            LOGGING_TASK_CORE_ID
        );

        esp_log_set_vprintf(logging_vprintf_async);
    }
}
