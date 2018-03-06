//
//  startup.h
//  
//
//  Created by Matthew Fonken on 1/4/17.
//
//

#ifndef startup_h
#define startup_h

/* System utilities */
#ifdef CHECK_TESTING
#include "../testing/ble_core_stubs.h"
#else
#include "utilities/usart_sp.h"
#include "utilities/clock_sp.h"
#endif

/* Peripheral/Sensor headers */
#include "../sensors/camera/cam_controller.h"
#include "utilities/PCA9534A.h"
#include "../sensors/imu/LSM9DS1.h"
#include "../sensors/touch/CPT112S.h"

#define ENABLED             1
#define DISABLED            0

#define SYS_CTL_LOC         0
#define RF_CTL_LOC          1
#define IMU_LOC             2
#define CAMERA_LOC          3
#define TOUCH_CTL_LOC       4

#define SYS_CTL_DEFAULT    ENABLED
#define RF_CTL_DEFAULT     ENABLED
#define IMU_DEFAULT        ENABLED
#define CAMERA_DEFAULT     ENABLED
#define TOUCH_CTL_DEFAULT  ENABLED

#define SYSTEM_DEFAULT                (\
    SYS_CTL_DEFAULT   << SYS_CTL_LOC  |\
    RF_CTL_DEFAULT    << RF_CTL_LOC   |\
    IMU_DEFAULT       << IMU_LOC      |\
    CAMERA_DEFAULT    << CAMERA_LOC   |\
    TOUCH_CTL_DEFAULT << TOUCH_CTL_LOC )

typedef struct
{
    /* System Utilities */
    uint8_t     system_controller:1;
    uint8_t     rf_controller:1;
    
    /* Sensors */
    uint8_t     imu:1;
    uint8_t     camera:1;
    uint8_t     touch:1;
    
    uint8_t     RESERVED:3;
} system_t;

extern system_t sys;

void startup( system_t * );

void Utilities_Init( void );
void Sensors_Init( void );

#endif /* startup_h */
