#include "esp_timer.h"
#include "pid.h"

void pid_compute(pid_t *pid) {
    int64_t now_us = esp_timer_get_time();

    int64_t dt_us = now_us - pid->last_time_us;
    if (dt_us <= PID_PERIOD_MIN_US) {
        return;
    }
    float dt_s = ((float) dt_us) / 1000000.0f;
    pid->last_time_us = now_us;

    pid->error_p  = (pid->setpoint - pid->current);

    pid->error_i += (pid->error_p * dt_s);

    pid->error_d  = (pid->error_p - pid->error_previous) / dt_s;

    if (pid->error_i > +pid->error_i_max) pid->error_i = +pid->error_i_max;
    if (pid->error_i < -pid->error_i_max) pid->error_i = -pid->error_i_max;

    pid->output = (
        (pid->kp * pid->error_p) +
        (pid->ki * pid->error_i) +
        (pid->kd * pid->error_d)
    );

    pid->error_previous = pid->error_p;
}
