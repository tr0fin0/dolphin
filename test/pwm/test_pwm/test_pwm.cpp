#if defined(ARDUINO)
#include <Arduino.h>
#endif

#include <unity.h>
#include "pwm.h"



void setUp(void) {}

void tearDown(void) {}



void pwm_pulse_us_normalize_deadband(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1500);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1550);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1450);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);
};

void pwm_pulse_us_normalize_maximum(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 2050);
    TEST_ASSERT_EQUAL_UINT16(2000, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 2000);
    TEST_ASSERT_EQUAL_UINT16(2000, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1950);
    TEST_ASSERT_EQUAL_UINT16(1950, pulse);
};

void pwm_pulse_us_normalize_minimum(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_us_normalize((pwm_pulse_t)  950);
    TEST_ASSERT_EQUAL_UINT16(1000, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1000);
    TEST_ASSERT_EQUAL_UINT16(1000, pulse);

    pulse = pwm_pulse_us_normalize((pwm_pulse_t) 1050);
    TEST_ASSERT_EQUAL_UINT16(1050, pulse);
};



void pwm_pulse_percentage_deadband(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_percentage((percentage_t) +10);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);

    pulse = pwm_pulse_percentage((percentage_t) 0);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);

    pulse = pwm_pulse_percentage((percentage_t)-10);
    TEST_ASSERT_EQUAL_UINT16(1500, pulse);
};


void pwm_pulse_percentage_maximum(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_percentage((percentage_t) +110);
    TEST_ASSERT_EQUAL_UINT16(2000, pulse);

    pulse = pwm_pulse_percentage((percentage_t) +100);
    TEST_ASSERT_EQUAL_UINT16(2000, pulse);

    pulse = pwm_pulse_percentage((percentage_t)  +90);
    TEST_ASSERT_EQUAL_UINT16(1950, pulse);
};

void pwm_pulse_percentage_minimum(void) {
    pwm_pulse_norm_t pulse;


    pulse = pwm_pulse_percentage((percentage_t)  -90);
    TEST_ASSERT_EQUAL_UINT16(1050, pulse);

    pulse = pwm_pulse_percentage((percentage_t) -100);
    TEST_ASSERT_EQUAL_UINT16(1000, pulse);

    pulse = pwm_pulse_percentage((percentage_t) -110);
    TEST_ASSERT_EQUAL_UINT16(1000, pulse);
};



int runTests(void) {
    UNITY_BEGIN();

    RUN_TEST(pwm_pulse_us_normalize_deadband);
    RUN_TEST(pwm_pulse_us_normalize_maximum);
    RUN_TEST(pwm_pulse_us_normalize_minimum);

    RUN_TEST(pwm_pulse_percentage_deadband);
    RUN_TEST(pwm_pulse_percentage_maximum);
    RUN_TEST(pwm_pulse_percentage_minimum);

    return UNITY_END();
}


#if defined(ARDUINO)
void setup(void) {
    runTests();
}
void loop(void) {}
#else
int main(void) {
    return runTests();
}
#endif
