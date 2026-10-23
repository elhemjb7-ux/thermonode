#include "unity.h"
#include "node.h"
#include "hal_double.h"

#define ADC_25  931u
#define ADC_70  1490u
#define ADC_130 2234u

void setUp(void)
{
    hal_double_reset();
    node_init(6000, 500);
    node_set_data_period_ms(0u);
}

void tearDown(void) {}

static void feed(uint16_t adc, int n)
{
    for (int i = 0; i < n; i++) {
        hal_double_push_adc(
            (uint16_t)(adc + (uint16_t)(i & 1))
        );
        hal_double_advance_ms(1000u);
        node_step();
    }
}

static void feed_same(uint16_t adc, int n)
{
    for (int i = 0; i < n; i++) {
        hal_double_push_adc(adc);
        hal_double_advance_ms(1000u);
        node_step();
    }
}

static void feed_seq(const uint16_t *values, int n)
{
    for (int i = 0; i < n; i++) {
        hal_double_push_adc(values[i]);
        hal_double_advance_ms(1000u);
        node_step();
    }
}

void test_EF04_alert_after_three_filtered_above(void)
{
    feed(ADC_25, 5);
    feed(ADC_70, 6);
    TEST_ASSERT_EQUAL(STATE_ALERT, node_state());
}

void test_EF05_out_of_range_enters_fault(void)
{
    feed(ADC_25, 5);
    feed(ADC_130, 1);
    TEST_ASSERT_EQUAL(STATE_FAULT, node_state());
}

void test_EF05_frozen_sensor_enters_fault(void)
{
    feed_same(ADC_25, 4);
    TEST_ASSERT_EQUAL(STATE_NORMAL, node_state());

    feed_same(ADC_25, 1);
    TEST_ASSERT_EQUAL(STATE_FAULT, node_state());
}

void test_EF05_recovers_to_normal_after_5_valid_samples(void)
{
    const uint16_t initial[] = {931u, 932u, 933u, 934u, 935u};
    feed_seq(initial, 5);

    feed(ADC_130, 1);
    TEST_ASSERT_EQUAL(STATE_FAULT, node_state());

    const uint16_t recovery[] = {931u, 932u, 933u, 934u};
    feed_seq(recovery, 4);
    TEST_ASSERT_EQUAL(STATE_FAULT, node_state());

    feed(935u, 1);
    TEST_ASSERT_EQUAL(STATE_NORMAL, node_state());

    /* Une trame de defaut, puis une trame de reprise. */
    TEST_ASSERT_EQUAL_INT(2, hal_double_send_calls());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF04_alert_after_three_filtered_above);
    RUN_TEST(test_EF05_out_of_range_enters_fault);
    RUN_TEST(test_EF05_frozen_sensor_enters_fault);
    RUN_TEST(test_EF05_recovers_to_normal_after_5_valid_samples);
    return UNITY_END();
}