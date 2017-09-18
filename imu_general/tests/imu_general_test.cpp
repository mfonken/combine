//
//  imu_general_test.cpp
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include "../utilities/i2c_sp.h"
#include "../sensors/LSM9DS1.h"
//#include "../kinetic/kinetic.h"

int main( int argc, char * args[] )
{

   	printf("Starting...\n");
	LSM9DS1_t lsm9;
	//kinetic_t kinetics;
	
	printf("Initing I2C...\n");
	I2C_Init();
    printf("Initing IMU...\n");
    IMU_Init(&lsm9);
    
/*

	printf("Initing kinetics...\n");

    	Kinetic_Init(&lsm9, &kinetics);


	//printf("R:%.2x > V:%.2x > A:%.2x\n", WHO_AM_I, IMU_WHO_AM_I_VAL, IMU_GetRegister(WHO_AM_I));
	//printf("R:%.2x > V:%.2x > A:%.2x\n", WHO_AM_I_M, MAG_WHO_AM_I_VAL, Mag_GetRegister(WHO_AM_I_M));

	if(IMU_Verify(&lsm9)) printf("IMU verification successful.\n");
	else printf("IMU verification failed!\n");
    
    	printf("Updating IMU...\n");
    	IMU_Update_All(&lsm9);
    
	for(int i = 0; i < 1000; i++)
	{
		Kinetic_Update_Rotation(&lsm9, &kinetics);
		printf("A<%.2f. %.2f,%.2f>\r\n", kinetics.rotation[0], kinetics.rotation[1], kinetics.rotation[2]);
	}
*/
    	return 0;
}
