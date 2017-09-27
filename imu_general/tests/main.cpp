//
//  imu_general_test.cpp
//  
//
//  Created by Matthew Fonken on 9/9/17.
//
//

#include <stdio.h>
#include "../utilities/i2c_sp.c"
#include "../sensors/LSM9DS1.c"
#include "../kinetic/kinetic.c"

int main( int argc, char * args[] )
{
    printf("Starting...\n");
    
    printf("Initing beacons.\n");
    cartesian2_t bns[2];
    bns[0].x = 400;
    bns[0].y = 100;
    bns[1].x = 100;
    bns[1].y = 400;
    
    printf("Initing IMU.\n");
    LSM9DS1_t imu;
    IMU_Init(&imu);
    
    printf("Initing kinetic.\n");
    kinetic_t kin;
    Kinetic_Init( &imu, &kin );
    
    printf("Updating position.\n");
    Kinetic_Update_Position( &imu, &kin, bns );
    
    printf("P:(%.2f, %.2f, %.2f)\n", kin.truePosition[0], kin.truePosition[1], kin.truePosition[2] );
    
    return 0;
}
