#include "acc.h"
#include "SEGGER_RTT.h"
#include <errno.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/dt-bindings/i2c/i2c.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/pinctrl.h>
#include <soc.h>





/* Create logging module for bluetooth*/
LOG_MODULE_REGISTER(ACCELEROMETER);
acc_meas acc_meas_values;

/* Modules initialization */
static const struct device *i2c_dev = DEVICE_DT_GET(I2C_NODE);
static const struct i2c_dt_spec dev_adxl345 = I2C_DT_SPEC_GET(adxl345_NODE);

/* Preparation of buffers */
static uint8_t i2c_buffer_read[6];
static uint8_t i2c_buffer_write[4];
static uint16_t i2c_buffer_values[3];

void adxl345_init(void){
	if (!device_is_ready(dev_adxl345.bus)){
		printk("I2C bus %s is not ready!\n\r",dev_adxl345.bus->name);
		return;
	}

	printk("I2C bus %s is READY!\n\r",dev_adxl345.bus->name);

	k_msleep(SLEEP_TIME_MS);

	i2c_buffer_write[0] = 0x00; // reset all bits
	i2c_buffer_write[1] = 0x08; // measure and wake up 8hz
	i2c_buffer_write[2] = 0x01; // data_format range= +- 4g
	i2c_buffer_write[3] = 0x0A; // Output Data Rate (Hz) 100 | Bandwidth (Hz) 50 

	if (i2c_reg_write_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_POWER_CTL, i2c_buffer_write[0])){
		printk(" Writting Error Buff[0]  \n");
		return;
	}
	if (i2c_reg_write_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_POWER_CTL, i2c_buffer_write[1])){
		printk(" Writting Error Buff[1]  \n");
		return;
	}
	if (i2c_reg_write_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATA_FORMAT, i2c_buffer_write[2])){
		printk(" Writting Error Buff[2]  \n");
		return;
	}
	if (i2c_reg_write_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_BW_RATE, i2c_buffer_write[3])){
		printk(" Writting Error Buff[3]  \n");
		return;
	}
}

int16_t* adxl345_read(void){
	do{
		k_msleep(100);

		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAX0, &i2c_buffer_read[0])){
			printk(" Reading Error Buff[0]  \n");
			return;
		}
		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAX1, &i2c_buffer_read[1])){
			printk(" Reading Error Buff[1]  \n");
			return;
		}
		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAY0, &i2c_buffer_read[2])){
			printk(" Reading Error Buff[2]  \n");
			return;
		}
		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAY1, &i2c_buffer_read[3])){
			printk(" Reading Error Buff[3]  \n");
			return;
		}
		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAZ0, &i2c_buffer_read[4])){
			printk(" Reading Error Buff[4]  \n");
			return;
		}
		if (i2c_reg_read_byte(i2c_dev, ADXL345_ADDRESS, ADXL345_DATAZ1, &i2c_buffer_read[3])){
			printk(" Reading Error Buff[5]  \n");
			return;
		}

		//printk(" ADXL345_DATA_FORMAT value = %d\n", i2c_buffer_read[0]);

		int16_t x,y,z;
		float xg,yg,zg;

		x = ((i2c_buffer_read[1]<<8)|i2c_buffer_read[0]);  
		y = ((i2c_buffer_read[3]<<8)|i2c_buffer_read[2]); 
		z = ((i2c_buffer_read[5]<<8)|i2c_buffer_read[4]);


		if(y > 120 && y < 255){
			y = y - 120;
			y = 135 - y;
		}
		if(x < 0){
			x = -x;
		}

		// ±4 g, 10-bit resolution = 7.8 mg/LBS
		xg = x * 0.00769 * 1000;
		yg = y * 0.00769 * 1000;
		zg = z * 0.00641 * 1000;

		x = (int16_t)xg;
		y = (int16_t)yg;
		z = (int16_t)zg;

		//printk(" X value = %d\n", x);
		//printk(" Y value = %d\n", y);
		//printk(" Z value = %d\n\n", z);

        i2c_buffer_values[0] = x;
        i2c_buffer_values[1] = y;
        i2c_buffer_values[2] = z;

        return i2c_buffer_values;

	}while(1);
}
