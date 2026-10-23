/* EF-04 : seuil et hysteresis ; EF-07 : limites de configuration */
#include "unity.h"
#include "threshold.h"

void setUp(void)
{
    threshold_set(6000, 500);
}

void tearDown(void) {}

void test_EF04_two_above_is_not_enough(void)
{
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(7000, false));
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(7000, false));
}

void test_EF04_three_consecutive_above_raise(void)
{
    threshold_step(7000, false);
    threshold_step(7000, false);
    TEST_ASSERT_EQUAL(THR_RAISE, threshold_step(7000, false));
}

void test_EF04_exactly_6000_does_not_count(void)
{
    threshold_step(6000, false);
    threshold_step(6000, false);
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(6000, false));
}

void test_EF04_6001_counts(void)
{
    threshold_step(6001, false);
    threshold_step(6001, false);
    TEST_ASSERT_EQUAL(THR_RAISE, threshold_step(6001, false));
}

void test_EF04_counter_resets_below_threshold(void)
{
    threshold_step(7000, false);
    threshold_step(7000, false);
    threshold_step(2500, false);
    threshold_step(7000, false);
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(7000, false));
}

void test_EF04_clear_below_hysteresis(void)
{
    TEST_ASSERT_EQUAL(THR_CLEAR, threshold_step(5499, true));
}

void test_EF04_at_hysteresis_bound_stays_alert(void)
{
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(5500, true));
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(7000, true));
}

void test_EF04_configured_hysteresis_is_used(void)
{
    threshold_set(6000, 300);
    TEST_ASSERT_EQUAL(THR_NONE, threshold_step(5700, true));
    TEST_ASSERT_EQUAL(THR_CLEAR, threshold_step(5699, true));
}

void test_EF07_set_checked_bounds(void)
{
    TEST_ASSERT_FALSE(threshold_set_checked(1900));
    TEST_ASSERT_EQUAL_INT32(6000, threshold_high());

    TEST_ASSERT_TRUE(threshold_set_checked(2000));
    TEST_ASSERT_EQUAL_INT32(2000, threshold_high());

    TEST_ASSERT_TRUE(threshold_set_checked(10000));
    TEST_ASSERT_EQUAL_INT32(10000, threshold_high());

    TEST_ASSERT_FALSE(threshold_set_checked(10100));
    TEST_ASSERT_EQUAL_INT32(10000, threshold_high());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF04_two_above_is_not_enough);
    RUN_TEST(test_EF04_three_consecutive_above_raise);
    RUN_TEST(test_EF04_exactly_6000_does_not_count);
    RUN_TEST(test_EF04_6001_counts);
    RUN_TEST(test_EF04_counter_resets_below_threshold);
    RUN_TEST(test_EF04_clear_below_hysteresis);
    RUN_TEST(test_EF04_at_hysteresis_bound_stays_alert);
    RUN_TEST(test_EF04_configured_hysteresis_is_used);
    RUN_TEST(test_EF07_set_checked_bounds);
    return UNITY_END();
}