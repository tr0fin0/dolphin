#include <unity.h>
#include "pwm.h"

void pwm_normalize_deadband(void) {
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_normalize((pwm_t) 1500));
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_normalize((pwm_t) 1550));
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_normalize((pwm_t) 1450));
};

void pwm_normalize_maximum(void) {
    TEST_ASSERT_EQUAL_UINT16(2000, pwm_normalize((pwm_t) 2050));
    TEST_ASSERT_EQUAL_UINT16(2000, pwm_normalize((pwm_t) 2000));
    TEST_ASSERT_EQUAL_UINT16(1950, pwm_normalize((pwm_t) 1950));
};

void pwm_normalize_minimum(void) {
    TEST_ASSERT_EQUAL_UINT16(1000, pwm_normalize((pwm_t)  950));
    TEST_ASSERT_EQUAL_UINT16(1000, pwm_normalize((pwm_t) 1000));
    TEST_ASSERT_EQUAL_UINT16(1050, pwm_normalize((pwm_t) 1050));
};

void pwm_percentage_deadband(void) {
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_percentage((pwm_percentage_t) +10));
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_percentage((pwm_percentage_t)   0));
    TEST_ASSERT_EQUAL_UINT16(1500, pwm_percentage((pwm_percentage_t) -10));
};

void pwm_percentage_maximum(void) {
    TEST_ASSERT_EQUAL_UINT16(2000, pwm_percentage((pwm_percentage_t) +110));
    TEST_ASSERT_EQUAL_UINT16(2000, pwm_percentage((pwm_percentage_t) +100));
    TEST_ASSERT_EQUAL_UINT16(1950, pwm_percentage((pwm_percentage_t)  +90));
};

void pwm_percentage_minimum(void) {
    TEST_ASSERT_EQUAL_UINT16(1050, pwm_percentage((pwm_percentage_t)  -90));
    TEST_ASSERT_EQUAL_UINT16(1000, pwm_percentage((pwm_percentage_t) -100));
    TEST_ASSERT_EQUAL_UINT16(1000, pwm_percentage((pwm_percentage_t) -110));
};

void app_main(void) {
    // setUp();
    UNITY_BEGIN();

    RUN_TEST(pwm_normalize_deadband);
    RUN_TEST(pwm_normalize_maximum);
    RUN_TEST(pwm_normalize_minimum);

    RUN_TEST(pwm_percentage_deadband);
    RUN_TEST(pwm_percentage_maximum);
    RUN_TEST(pwm_percentage_minimum);

    UNITY_END();
    // tearDown();
}
