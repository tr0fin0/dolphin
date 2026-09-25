#include "controller.h"
#include "esc.h"
#include "esp_rom_sys.h"

static void controller_set_movement(
    pwm_percentage_t power_left,
    pwm_percentage_t power_right,
    uint16_t duration_us
) {
    esc_set_pwm(pwm_percentage(power_left),  ESC_L);
    esc_set_pwm(pwm_percentage(power_right), ESC_R);
    esp_rom_delay_us(duration_us);

    esc_set_pwm_mix_neutral();
}

void controller_set_rotation(pwm_percentage_t power, uint16_t duration_us) {
    if (power > 0) {
        controller_set_movement(+power, +power, duration_us);
    } else if (power < 0) {
        controller_set_movement(-power, -power, duration_us);
    } else {
        return;
    }
}

void controller_set_translation(pwm_percentage_t power, uint16_t duration_us) {
    if (power > 0) {
        controller_set_movement(+power, -power, duration_us);
    } else if (power < 0) {
        controller_set_movement(-power, +power, duration_us);
    } else {
        return;
    }
}
