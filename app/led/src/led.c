#include "led.h"

LOG_MODULE_REGISTER(LEDS);

static ret_status led_pin_configure(const struct gpio_dt_spec led);
void led_init(led_choice selected_led);
void led_init_all_diodes(void);
void led_toggle(led_choice selected_led);
void led_set_state(led_choice selected_led, led_state led_state);


/* Function initialize selected led */
void led_init(led_choice selected_led)
{ 
    ret_status ret;
    switch (selected_led)
    {
    case LED0:
        ret = led_pin_configure(led_0);
        break;
    case LED1:
        ret = led_pin_configure(led_1);
        break;
    case LED2:
        ret = led_pin_configure(led_2);
        break;
    case LED3:
        ret = led_pin_configure(led_3);
        break;
    default:
        ret = ERROR;
        break;
    }

    if(ret == SUCCESS){
        LOG_INF("LED%d Init success", selected_led);
    }
    else{
        LOG_ERR("LED%d Init error", selected_led);
    }

}


/* Function initialize all leds */
void led_init_all_diodes(void)
{
    for(led_choice selected_led = LED0; selected_led <= LED3; selected_led++){
        led_init(selected_led);
    }
}

/* Function configure selected pin */
static ret_status led_pin_configure(const struct gpio_dt_spec led)
{
    int ret;
    if (!device_is_ready(led.port)) {
		return ERROR;
	}
    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        return ERROR;
    }
    return SUCCESS;
}

/* Function toggle led */
void led_toggle(led_choice selected_led)
{
	int ret;
    switch (selected_led)
    {
    case LED0:
        ret = gpio_pin_toggle_dt(&led_0);
        break;
    case LED1:
        ret = gpio_pin_toggle_dt(&led_1);
        break;
    case LED2:
        ret = gpio_pin_toggle_dt(&led_2);
        break;
    case LED3:
        ret = gpio_pin_toggle_dt(&led_3);
        break;
    default:
        ret = -1;
        break;
    }
    if(ret < 0){
        LOG_ERR("LED%d Toggle error", selected_led);
    }
    else{
        LOG_INF("LED%d Toggle success", selected_led);
    }
}

/* Function set selected state on LED */
void led_set_state(led_choice selected_led, led_state led_state)
{
    int ret;
    switch (selected_led)
    {
    case LED0:
        ret = gpio_pin_set_dt(&led_0, led_state);
        break;
    case LED1:
        ret = gpio_pin_set_dt(&led_1, led_state);
        break;
    case LED2:
        ret = gpio_pin_set_dt(&led_2, led_state);
        break;
    case LED3:
        ret = gpio_pin_set_dt(&led_3, led_state);
        break;
    default:
        ret = -1;
        break;
    }
    
    if(ret < 0){
        LOG_ERR("LED%d Set state error", selected_led);
    }
    else{
        LOG_INF("LED%d Set state success", selected_led);
    }
}
