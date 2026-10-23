#include "unity.h"
#include "node.h"
#include "hal_double.h"
#include <stdio.h>
#include <string.h>

void setUp(void)
{
    hal_double_reset();
    node_init(6000, 500);
    node_set_data_period_ms(0u);
}

void tearDown(void) {}

static void feed_seq(const uint16_t *values, int n)
{
    for (int i = 0; i < n; i++) {
        hal_double_push_adc(values[i]);
        hal_double_advance_ms(1000u);
        node_step();
    }
}

void test_EF06_periodic_data_frame_every_10s(void)
{
    node_set_data_period_ms(NODE_DATA_PERIOD_MS);

    const uint16_t values[] = {
        931u, 932u, 933u, 934u, 935u,
        931u, 932u, 933u, 934u, 935u, 931u
    };
    feed_seq(values, 11);

    TEST_ASSERT_EQUAL_INT(1, hal_double_send_calls());

    size_t len;
    const uint8_t *frame = hal_double_last_frame(&len);
    TEST_ASSERT_NOT_NULL(frame);
    TEST_ASSERT_NOT_NULL(
        strstr((const char *)frame, "\"state\":\"NORMAL\"")
    );
}

void test_ENF05_frames_buffered_and_resent_in_order_after_outage(void)
{
    node_set_data_period_ms(1000u);
    hal_double_set_connected(false);

    const uint16_t values[] = {
        931u, 932u, 933u, 934u, 935u,
        936u, 937u, 938u, 939u, 940u
    };
    feed_seq(values, 10);
    TEST_ASSERT_EQUAL_INT(0, hal_double_send_calls());

    hal_double_set_connected(true);
    const uint16_t next[] = {931u};
    feed_seq(next, 1);

    TEST_ASSERT_EQUAL_INT(11, hal_double_send_calls());

    for (int i = 0; i < 11; i++) {
        size_t len;
        const uint8_t *frame = hal_double_frame(i, &len);
        char expected[24];

        TEST_ASSERT_NOT_NULL(frame);
        snprintf(expected, sizeof expected, "\"seq\":%d,", i);
        TEST_ASSERT_NOT_NULL_MESSAGE(
            strstr((const char *)frame, expected),
            expected
        );
    }
}

void test_ENF05_no_send_while_disconnected(void)
{
    node_set_data_period_ms(1000u);
    hal_double_set_connected(false);

    const uint16_t values[] = {931u, 932u, 933u, 934u, 935u};
    feed_seq(values, 3);
    TEST_ASSERT_EQUAL_INT(0, hal_double_send_calls());

    feed_seq(values + 3, 2);
    TEST_ASSERT_EQUAL_INT(0, hal_double_send_calls());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF06_periodic_data_frame_every_10s);
    RUN_TEST(test_ENF05_frames_buffered_and_resent_in_order_after_outage);
    RUN_TEST(test_ENF05_no_send_while_disconnected);
    return UNITY_END();
}