#include "ema.h"

uint16_t ema_compute_circular(
    const uint16_t *buffer,
    uint8_t capacity,
    uint8_t head,
    float alpha
) {
    if (buffer == NULL || capacity == 0 || head >= capacity) {
        return 0;
    }

    uint8_t position = head;
    float average = (float) buffer[position];

    for (uint8_t i = 1; i < capacity; i++) {
        position = (head + i) % capacity;
        average = (
            (alpha * (float) buffer[position]) + ((1.0f - alpha) * average)
        );
    }

    return (uint16_t) average;
};
