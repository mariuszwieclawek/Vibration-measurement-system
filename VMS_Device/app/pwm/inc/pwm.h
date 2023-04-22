#ifndef _PWM_H_
#define _PWM_H_

/* Function initialize pwm */
void pwm_init(void);
/* Set PWM value for vibration motor */
ret_status pwm_set_motor_value(uint8_t value);

#endif /* _PWM_H_ */
