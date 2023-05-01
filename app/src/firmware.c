#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

#include "core/system.h"
#include "vga.h"

#define LED_PORT (GPIOC)
#define LED_PIN (GPIO9)

static void gpio_setup(void) {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOC);
    gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);

    gpio_mode_setup(VGA_RED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, VGA_RED_PIN);
    // gpio_mode_setup(VGA_GREEN_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, VGA_GREEN_PIN);
    gpio_mode_setup(VGA_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, VGA_BLUE_PIN);

    /*
     * | Pin  | AF1      | AF2       | AF3 | AF4 |
     * | ---- | -------- | --------- | --- | --- |
     * | PA0  | TIM2_CH1 |           |     |     |
     * | PA1  | TIM2_CH2 |           |     |     |
     * | PA2  | TIM2_CH3 |           |     |     |
     * | PA3  | TIM2_CH4 |           |     |     |
     * | PA8  | TIM1_CH1 |           |     |     |
     * | PA9  | TIM1_CH2 | TIM1_BKIN |     |     |
     * | PA10 | TIM1_CH3 |           |     |     |
     * | PA11 | TIM1_CH4 | TIM1_BKIN |     |     |
     * | PA15 | TIM2_CH1 |           |     |     |
     */

    /* Configure HSYNC pin in alternative mode (TIM1_CH1) for HSYNC output */
    /* Decrease output slew rate to handle higher toggle rate */
    gpio_mode_setup(HSYNC_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, HSYNC_PIN);
    gpio_set_af(HSYNC_PORT, GPIO_AF1, HSYNC_PIN);
    gpio_set_output_options(HSYNC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, HSYNC_PIN);

    /* Configure VSYNC pin in alternative mode (TIM2_CH1) for VSYNC output */
    /* Decrease output slew rate to handle higher toggle rate */
    gpio_mode_setup(VSYNC_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, VSYNC_PIN);
    gpio_set_af(VSYNC_PORT, GPIO_AF1, VSYNC_PIN);
    gpio_set_output_options(VSYNC_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, VSYNC_PIN);
}

int main(void) {
    system_setup();
    gpio_setup();
    vga_setup();

    uint64_t start_time = system_get_ticks();

    /* Infinte loop */
    for (;;) {
        // if (true){
        if (system_get_ticks() - start_time >= 8) {
            gpio_toggle(LED_PORT, LED_PIN);

            gpio_toggle(VGA_RED_PORT, VGA_RED_PIN);
            // gpio_toggle(VGA_GREEN_PORT, VGA_GREEN_PIN);
            gpio_toggle(VGA_BLUE_PORT, VGA_BLUE_PIN);

            start_time = system_get_ticks();
        }
    }

    // Never return
    return 0;
}
