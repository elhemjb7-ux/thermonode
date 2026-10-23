#include "unity.h"
#include "node.h"
#include "hal_double.h"

void setUp(void)
{
    hal_double_reset();
    node_init(6000, 500);
    node_set_data_period_ms(0u);
}

void tearDown(void) {}

void test_INIT_goes_to_NORMAL_on_first_step(void)
{
    TEST_ASSERT_EQUAL(STATE_INIT, node_state());

    hal_double_push_adc(931u);
    hal_double_advance_ms(1000u);
    node_step();

    TEST_ASSERT_EQUAL(STATE_NORMAL, node_state());
}

void test_EF01_default_acquisition_period_is_1000ms(void)
{
    TEST_ASSERT_EQUAL_UINT32(1000u, node_acq_period_ms());
}

void test_watchdog_kicked_each_step(void)
{
    for (int i = 0; i < 3; i++) {
        hal_double_push_adc((uint16_t)(931 + i));
        hal_double_advance_ms(1000u);
        node_step();
    }

    TEST_ASSERT_EQUAL_INT(3, hal_double_watchdog_kicks());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_INIT_goes_to_NORMAL_on_first_step);
    RUN_TEST(test_EF01_default_acquisition_period_is_1000ms);
    RUN_TEST(test_watchdog_kicked_each_step);
    return UNITY_END();
}