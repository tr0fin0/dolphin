#include "driver/gpio.h"
#include "esp_err.h"
#include "ir.h"
#include "logging.h"
#include "portmacro.h"
#include "pinout.h"
#include "soc/gpio_reg.h"

static ir_t ir = {
    .name = "JSumo Micro",
    .state_names = {
        [IR_STATE_STANDBY] = "STANDBY",
        [IR_STATE_START]   = "START",
        [IR_STATE_STOP]    = "STOP",
    },
    .pin = PIN_IRR,
    .state = IR_STATE_STANDBY,
};

/**
 * @brief GPIO Interrupt Service Routine for the IR signal capture.
 *
 * When in @ref IR_STATE_STOP , only system power reset will reinitialize the
 * infrared receiver.
 *
 * @param arg IR pin as void *.
 *
 * @note GPIO level is read with direct ESP32 register access
 * `REG_READ(GPIO_IN_REG)` providing faster and ISR-safe input sampling.
 */
static void IRAM_ATTR ir_isr(void *arg) {
    pin_t pin = (pin_t) (uintptr_t) arg;
    uint32_t signal_start = (REG_READ(GPIO_IN_REG) >> (pin)) & 0x1;

    switch (ir.state) {
        case IR_STATE_STANDBY:
            if ( signal_start)   { ir.state = IR_STATE_START; };
            break;

        case IR_STATE_START:
            if (!signal_start)   { ir.state = IR_STATE_STOP; };
            break;

        case IR_STATE_STOP:
            break;

        default:
            break;
    }
};

const char *ir_get_name(void) {
    return ir.name;
};

ir_state_t ir_get_state(void) {
    return ir.state;
};

const char *ir_get_state_name(void) {
    return ir.state_names[ir.state];
};

void ir_init(void) {
    ESP_ERROR_CHECK_WITHOUT_ABORT(gpio_install_isr_service(ESP_INTR_FLAG_IRAM));

    gpio_config_t pin_config = {
        .pin_bit_mask   = (1ULL << ir.pin),
        .mode           = GPIO_MODE_INPUT,
        .pull_up_en     = GPIO_PULLUP_DISABLE,
        .pull_down_en   = GPIO_PULLDOWN_DISABLE,
        .intr_type      = GPIO_INTR_ANYEDGE
    };

    ESP_ERROR_CHECK(gpio_config(&pin_config));

    ESP_ERROR_CHECK(
        gpio_isr_handler_add(ir.pin, ir_isr, (void *) (uintptr_t) ir.pin)
    );

    ir.state = IR_STATE_STANDBY;
    LOG_I("%s initialized to state %s", ir_get_name(), ir_get_state_name());
};
