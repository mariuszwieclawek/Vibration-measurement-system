#ifndef _LED_H_
#define _LED_H_

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "common_types.h"

typedef enum
{
    LED0,
    LED1,
    LED2,
    LED3,
} led_choice;

typedef enum
{
    INACTIVE,
    ACTIVE,
} led_state;



/* The devicetree node identifier for the "led(n)" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)

/* Led structures indicating port and pin where led is connected */
static const struct gpio_dt_spec led_0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led_3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);

/* Function initialize selected led */
void led_init(led_choice selected_led);
void led_init_all_diodes(void);
void led_toggle(led_choice selected_led);
void led_set_state(led_choice selected_led, led_state led_state);

#endif /* _LED_H_ */
