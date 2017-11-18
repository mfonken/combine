//
//  LSM9DS1_types.h
//
//
//  Created by Matthew Fonken on 6/22/17.
//
//

#ifndef LSM9DS1_types_h
#define LSM9DS1_types_h

#ifdef __cplusplus
extern "C" {
  #endif

  #include "LSM9DS1_regs.h"

  typedef struct
  {
    double 	accel[3];
    double 	gyro[3];
    double 	mag[3];

    double  accel_res;
    double 	gyro_res;
    double 	mag_res;

    double 	accel_bias[3];
    double 	gyro_bias[3];
    double 	mag_bias[3]; 

    double	roll;
    double	pitch;
    double	yaw;
  } imu_t;

  typedef struct
  {
    imu_t               data;
    LSM9DS1_cfg_t       settings;
  } LSM9DS1_t;

  #ifdef __cplusplus
}
#endif

#endif /* LSM9DS1_types_h */
