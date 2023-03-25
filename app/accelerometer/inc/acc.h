#ifndef _ACC_H_
#define _ACC_H_

#include <zephyr/logging/log.h>

typedef struct
{
	float x_axis_val;
	float y_axis_val;
	float z_axis_val;
}acc_meas;

extern acc_meas acc_meas_values;

#endif /* _ACC_H_ */

