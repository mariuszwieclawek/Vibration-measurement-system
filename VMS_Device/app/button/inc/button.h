#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "common_types.h"
#include "led.h"

extern int button_counter;

typedef enum
{
    BUTTON0,
    BUTTON1,
    BUTTON2,
    BUTTON3,
} button_choice;


/* Devicetree node identifier for the node with alias "sw(n)" */
#define SW0_NODE	DT_ALIAS(sw0)
#define SW1_NODE	DT_ALIAS(sw1) 
#define SW2_NODE	DT_ALIAS(sw2) 
#define SW3_NODE	DT_ALIAS(sw3) 

/* Button structures indicating port and pin where button is connected */
static const struct gpio_dt_spec button_0 = GPIO_DT_SPEC_GET(SW0_NODE, gpios);
static const struct gpio_dt_spec button_1 = GPIO_DT_SPEC_GET(SW1_NODE, gpios);
static const struct gpio_dt_spec button_2 = GPIO_DT_SPEC_GET(SW2_NODE, gpios);
static const struct gpio_dt_spec button_3 = GPIO_DT_SPEC_GET(SW3_NODE, gpios);

/* Function initialize selected button */
void button_init(button_choice selected_button);
void button_init_all_switches(void);
void button_init_interrupt(button_choice selected_button);
void button_init_interrupts_for_all_switches(void);


#endif /* _BUTTON_H_ */