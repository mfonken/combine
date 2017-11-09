#include <stdio.h>
#include <string>
#include "../utilities/i2c_sp.c"
#include "../sensors/LSM9DS1.c"
#include "../kinetic/kinetic.c"

cartesian2_t bns[2];
LSM9DS1_t imu;
kinetic_t kin;

void imu_general_setup( std::string msg )
{

    printf("Starting...\n");
    printf("Initing I2C...\n");
    I2C_Init();
    
    printf("Initing beacons.\n");
    
    bns[0].x = 400;
    bns[0].y = 100;
    bns[1].x = 100;
    bns[1].y = 400;
    
    printf("Initing IMU.\n");
    IMU_Init(&imu);
    
    printf("Initing kinetic.\n");
    Kinetic_Init( &imu, &kin );
}

void imu_general_main( std::string msg )
{
    printf("Updating position.\n");
    Kinetic_Update_Position( &imu, &kin, bns );
    char buff[100];
    sprintf(buff, "P:(%.2f, %.2f, %.2f)\n", kin.truePosition[0], kin.truePosition[1], kin.truePosition[2] );
    msg = buff;
}

