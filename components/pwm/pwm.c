#include "pwm.h"

pwm_norm_t pwm_normalize(pwm_t pwm) {
    if (pwm > (pwm_t) PWM_MAXIMUM_US - PWM_DEADBAND_US) {
        return (pwm_norm_t) PWM_MAXIMUM_US;
    }

    if (pwm < (pwm_t) PWM_MINIMUM_US + PWM_DEADBAND_US) {
        return (pwm_norm_t) PWM_MINIMUM_US;
    }

    pwm_t diff = pwm - (pwm_t) PWM_NEUTRAL_US;
    if (
        (diff >= -(pwm_t) PWM_DEADBAND_US) &&
        (diff <= +(pwm_t) PWM_DEADBAND_US)
    ) {
        return (pwm_norm_t) PWM_NEUTRAL_US;
    }

    return (pwm_norm_t) pwm;
}

pwm_norm_t pwm_percentage(pwm_percentage_t pwm_percentage) {
    return pwm_normalize((pwm_t) PWM_NEUTRAL_US + 5 * pwm_percentage);
}
