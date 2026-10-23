#include "unity.h"
#include "node.h"
#include "hal_double.h"
#include "frame.h"
#include <stdio.h>
#include <string.h>

void setUp(void)
{
    hal_double_reset();
    node_init(6000, 500);
    node_set_data_period_ms(0u);
}

void tearDown(void) {}

static void queue_signed(const char *payload)
{
    char command[128];
    size_t n = strlen(payload);

    snprintf(
        command, sizeof command,
        "%.*s,\"crc\":\"%04X\"}",
        (int)n - 1, payload,
        (unsigned int)crc16_ccitt_false(
            (const uint8_t *)payload, n
        )
    );
    hal_double_queue_command(command);
}

static void assert_ack_contains(const char *expected)
{
    size_t len;
    const uint8_t *ack = hal_double_last_ack(&len);

    TEST_ASSERT_EQUAL_INT(1, hal_double_ack_calls());
    TEST_ASSERT_NOT_NULL(ack);
    TEST_ASSERT_TRUE(len > 0u);
    TEST_ASSERT_NOT_NULL_MESSAGE(
        strstr((const char *)ack, expected), expected
    );
}

void test_EF07_command_set_threshold_ok_and_ack(void)
{
    queue_signed("{\"cmd\":\"set_threshold\",\"value\":65}");
    node_poll_commands();
    assert_ack_contains("\"result\":\"ok\"");
}

void test_EF07_command_out_of_range_is_nacked(void)
{
    queue_signed("{\"cmd\":\"set_threshold\",\"value\":101}");
    node_poll_commands();
    assert_ack_contains("\"reason\":\"range\"");
}

void test_EF07_command_bad_crc_is_ignored_silently(void)
{
    hal_double_queue_command(
        "{\"cmd\":\"set_threshold\",\"value\":65,\"crc\":\"0000\"}"
    );
    node_poll_commands();
    TEST_ASSERT_EQUAL_INT(0, hal_double_ack_calls());
}

void test_EF08_set_period_changes_acquisition_period(void)
{
    queue_signed("{\"cmd\":\"set_period\",\"value\":5}");
    node_poll_commands();
    TEST_ASSERT_EQUAL_UINT32(5000u, node_acq_period_ms());
    assert_ack_contains("\"result\":\"ok\"");
}

void test_EF08_set_period_out_of_range_is_nacked(void)
{
    queue_signed("{\"cmd\":\"set_period\",\"value\":61}");
    node_poll_commands();
    assert_ack_contains("\"reason\":\"range\"");
    TEST_ASSERT_EQUAL_UINT32(1000u, node_acq_period_ms());
}

void test_EI03_unknown_command_is_nacked_unknown(void)
{
    queue_signed("{\"cmd\":\"reboot\",\"value\":1}");
    node_poll_commands();
    assert_ack_contains("\"reason\":\"unknown\"");
}

void test_EI03_command_without_cmd_is_nacked_format(void)
{
    queue_signed("{\"value\":1}");
    node_poll_commands();
    assert_ack_contains("\"reason\":\"format\"");
}

void test_EI03_command_without_value_is_nacked_format(void)
{
    queue_signed("{\"cmd\":\"set_period\"}");
    node_poll_commands();
    assert_ack_contains("\"reason\":\"format\"");
}

void test_no_command_pending_does_nothing(void)
{
    node_poll_commands();
    TEST_ASSERT_EQUAL_INT(0, hal_double_ack_calls());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF07_command_set_threshold_ok_and_ack);
    RUN_TEST(test_EF07_command_out_of_range_is_nacked);
    RUN_TEST(test_EF07_command_bad_crc_is_ignored_silently);
    RUN_TEST(test_EF08_set_period_changes_acquisition_period);
    RUN_TEST(test_EF08_set_period_out_of_range_is_nacked);
    RUN_TEST(test_EI03_unknown_command_is_nacked_unknown);
    RUN_TEST(test_EI03_command_without_cmd_is_nacked_format);
    RUN_TEST(test_EI03_command_without_value_is_nacked_format);
    RUN_TEST(test_no_command_pending_does_nothing);
    return UNITY_END();
}