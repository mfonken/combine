//
//  imu_general_test.cpp
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include <stdint.h>
#include "../utilities/i2c_sp.c"
#include "../sensors/LSM9DS1_regs.h"

#define IMU_ADDR 0x6b
#define MAG_ADDR 0x1e

int main( int argc, char * args[] )
{
    	printf("Starting...\n");

    	printf("Initing I2C...\n");
	I2C_Init();
	
	uint8_t len = 1;
	uint8_t data[len];
	uint8_t reg = WHO_AM_I;
	printf("Reading register:%.2x\n",reg);
    	data[0] = I2C_Read_Reg(IMU_ADDR, reg);
	printf("Read value is %.2x\n", data[0]);

	reg = WHO_AM_I_M;
	printf("Reading register:%.2x\n",reg);
    	data[0] = I2C_Read_Reg(MAG_ADDR, reg);
	printf("Read value is %.2x\n", data[0]);
    	return 0;
}
