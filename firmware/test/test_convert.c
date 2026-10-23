/* EF-02, EF-05 : conversion et limites physiques */
#include "unity.h"
#include "convert.h"

void setUp(void) {}
void tearDown(void) {}

static int32_t conv(uint16_t adc, bool *v)
{
    *v = true;
    return temp_from_adc(adc, v);
}

void test_EF02_adc_0_gives_minus_50_00(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(-5000, conv(0u, &v));
    TEST_ASSERT_FALSE(v);
}

void test_EF02_adc_2048_gives_115_00(void)
{
    bool v;
    TEST_ASSERT_INT32_WITHIN(1, 11500, conv(2048u, &v));
    TEST_ASSERT_TRUE(v);
}

void test_EF02_adc_931_gives_25_00(void)
{
    bool v;
    TEST_ASSERT_INT32_WITHIN(3, 2500, conv(931u, &v));
    TEST_ASSERT_TRUE(v);
}

void test_EF02_adc_4095_is_out_of_physical_range(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(27991, conv(4095u, &v));
    TEST_ASSERT_FALSE(v);
}

void test_EI01_EF02_adc_4096_is_invalid_code(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(0, conv(4096u, &v));
    TEST_ASSERT_FALSE(v);
}

void test_EI01_EF02_adc_65535_is_invalid_code(void)
{
    bool v;
    conv(65535u, &v);
    TEST_ASSERT_FALSE(v);
}

void test_EF05_adc_124_is_below_minus_40(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(-4001, conv(124u, &v));
    TEST_ASSERT_FALSE(v);
}

void test_EF05_adc_125_is_valid_lower_bound(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(-3993, conv(125u, &v));
    TEST_ASSERT_TRUE(v);
}

void test_EF05_adc_2172_is_valid_upper_bound(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(12499, conv(2172u, &v));
    TEST_ASSERT_TRUE(v);
}

void test_EF05_adc_2173_is_above_125(void)
{
    bool v;
    TEST_ASSERT_EQUAL_INT32(12507, conv(2173u, &v));
    TEST_ASSERT_FALSE(v);
}

void test_EF05_range_check_bounds(void)
{
    TEST_ASSERT_FALSE(range_check(-4001));
    TEST_ASSERT_TRUE(range_check(-4000));
    TEST_ASSERT_TRUE(range_check(12500));
    TEST_ASSERT_FALSE(range_check(12501));
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF02_adc_0_gives_minus_50_00);
    RUN_TEST(test_EF02_adc_2048_gives_115_00);
    RUN_TEST(test_EF02_adc_931_gives_25_00);
    RUN_TEST(test_EF02_adc_4095_is_out_of_physical_range);
    RUN_TEST(test_EI01_EF02_adc_4096_is_invalid_code);
    RUN_TEST(test_EI01_EF02_adc_65535_is_invalid_code);
    RUN_TEST(test_EF05_adc_124_is_below_minus_40);
    RUN_TEST(test_EF05_adc_125_is_valid_lower_bound);
    RUN_TEST(test_EF05_adc_2172_is_valid_upper_bound);
    RUN_TEST(test_EF05_adc_2173_is_above_125);
    RUN_TEST(test_EF05_range_check_bounds);
    return UNITY_END();
}