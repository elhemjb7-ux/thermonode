#include "unity.h"
#include "node.h"
#include "hal_double.h"

#define ADC_25 931u
#define ADC_70 1490u

void setUp(void)
{
    hal_double_reset();
    node_init(6000, 500);
    node_set_data_period_ms(0u);
}

void tearDown(void) {}

/* Alterner les codes evite de simuler un capteur fige. */
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

void test_EF04_alert_after_three_above(void)
{
    feed(ADC_25, 5);
    feed(ADC_70, 6);
    TEST_ASSERT_EQUAL(STATE_ALERT, node_state());
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_EF04_alert_after_three_above);
    return UNITY_END();
}
