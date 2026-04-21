#include "pwm.h"



pwm_pulse_norm_t IRAM_ATTR pwm_pulse_us_normalize(pwm_pulse_t pulse_us) {
    if (pulse_us > (pwm_pulse_t) PWM_MAXIMUM_US - PWM_DEADBAND_US) {
        return (pwm_pulse_norm_t) PWM_MAXIMUM_US;
    }

    if (pulse_us < (pwm_pulse_t) PWM_MINIMUM_US + PWM_DEADBAND_US) {
        return (pwm_pulse_norm_t) PWM_MINIMUM_US;
    }

    pwm_pulse_t diff = pulse_us - (pwm_pulse_t) PWM_NEUTRAL_US;
    if (
        (diff >= -(pwm_pulse_t) PWM_DEADBAND_US) &&
        (diff <= +(pwm_pulse_t) PWM_DEADBAND_US)
    ) {
        return (pwm_pulse_norm_t) PWM_NEUTRAL_US;
    }

    return (pwm_pulse_norm_t) pulse_us;
};

pwm_pulse_norm_t pwm_pulse_percentage(percentage_t percentage) {
    if (percentage > +100) {
        percentage = +100;
    }

    if (percentage < -100) {
        percentage = -100;
    }

    return pwm_pulse_us_normalize(
        (pwm_pulse_t) PWM_NEUTRAL_US + 5 * percentage
    );
}
