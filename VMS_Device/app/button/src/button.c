#include "button.h"

LOG_MODULE_REGISTER(BUTTONS);

int button_counter = 0;

void button_init(button_choice selected_button);
void button_init_all_switches(void);
static ret_status button_pin_configure(const struct gpio_dt_spec button);
void button_init_interrupt(button_choice selected_button);
void button_init_interrupts_for_all_switches(void);
static ret_status button_interrupt_configure(const struct gpio_dt_spec button, struct gpio_callback *button_cb_data, gpio_callback_handler_t button_pressed);


/* Define the callback function for BUTTON0 */
void button0_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // TO DO
    button_counter++;
    led_set_state(LED0, ACTIVE);
}

/* Define the callback function for BUTTON0 */
void button1_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // TO DO
    led_set_state(LED0, INACTIVE);
}

/* Define the callback function for BUTTON0 */
void button2_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // TO DO
    led_toggle(LED2);
}

/* Define the callback function for BUTTON0 */
void button3_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    // TO DO
    led_toggle(LED3);
}

/* Define a variable of type static struct gpio_callback. Variable will hold information such as the pin number and the function to be called when an interrupt occurs (callback function). */
static struct gpio_callback button0_cb_data;
static struct gpio_callback button1_cb_data;
static struct gpio_callback button2_cb_data;
static struct gpio_callback button3_cb_data;
 

/* Function initialize selected button */
void button_init(button_choice selected_button)
{ 
    ret_status ret;
    switch (selected_button)
    {
    case BUTTON0:
        ret = button_pin_configure(button_0);
        break;
    case BUTTON1:
        ret = button_pin_configure(button_1);
        break;
    case BUTTON2:
        ret = button_pin_configure(button_2);
        break;
    case BUTTON3:
        ret = button_pin_configure(button_3);
        break;
    default:
        ret = ERROR;
        break;
    }

    if(ret == SUCCESS){
        LOG_INF("BUTTON%d Init success", selected_button);
    }
    else{
        LOG_ERR("BUTTON%d Init error", selected_button);
    }

}

/* Function initialize all buttons */
void button_init_all_switches(void)
{
    for(button_choice selected_button = BUTTON0; selected_button <= BUTTON3; selected_button++){
        button_init(selected_button);
    }
}


/* Function configure selected pin */
static ret_status button_pin_configure(const struct gpio_dt_spec button)
{
    int ret;
	if (!device_is_ready(button.port)) {
		return ERROR;
	}
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		return ERROR;
	}
    return SUCCESS;
}


/* Function initialize interrupt for selected button pin*/
void button_init_interrupt(button_choice selected_button)
{
    ret_status ret;
    switch (selected_button)
    {
    case BUTTON0:
        ret = button_interrupt_configure(button_0, &button0_cb_data, button0_pressed);
        break;
    case BUTTON1:
        ret = button_interrupt_configure(button_1, &button1_cb_data, button1_pressed);
        break;
    case BUTTON2:
        ret = button_interrupt_configure(button_2, &button2_cb_data, button2_pressed);
        break;
    case BUTTON3:
        ret = button_interrupt_configure(button_3, &button3_cb_data, button3_pressed);
        break;
    default:
        ret = ERROR;
        break;
    }

    if(ret == SUCCESS){
        LOG_INF("BUTTON%d Interrupt init success", selected_button);
    }
    else{
        LOG_ERR("BUTTON%d Interrupt init error", selected_button);
    }
}


/* Function initialize interrupts for all buttons */
void button_init_interrupts_for_all_switches(void)
{
    for(button_choice selected_button = BUTTON0; selected_button <= BUTTON3; selected_button++){
        button_init_interrupt(selected_button);
    }
}


/* Function configure pin interrupt, initialize callback variable and setting handler function for callback */
static ret_status button_interrupt_configure(const struct gpio_dt_spec button, struct gpio_callback *button_cb_data, gpio_callback_handler_t button_pressed)
{
    int ret;
    /*  Configure the interrupt on the button's pin */
	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE );
    if(ret < 0){
        return ERROR;
    }

	/*  Initialize the static struct gpio_callback variable   */
    gpio_init_callback(button_cb_data, button_pressed, BIT(button.pin)); 	
	
	/*  Add the callback function by calling gpio_add_callback()   */
	ret = gpio_add_callback(button.port, button_cb_data);
    if(ret < 0){
        return ERROR;
    }

    return SUCCESS;
}
