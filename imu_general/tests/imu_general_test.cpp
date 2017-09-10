//
//  imu_general_test.cpp
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include "../sensors/LSM9DS1.c"

int main( int argc, char * args[] )
{
    printf("Starting...\n");

    printf("Initing IMU...\n");
    LSM9DS1_t lsm9;
    IMU_Init(&lsm9);
    
    printf("Updating IMU...\n");
    IMU_Update_All(&lsm9);
    
    return 0;
}
