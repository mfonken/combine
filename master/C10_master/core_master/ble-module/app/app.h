/*
 * app.h
 *
 *  Created on: Jan 9, 2017
 *      Author: Matthew Fonken
 */

#ifndef SYSTEM_APP_H_
#define SYSTEM_APP_H_

#include <stdint.h>
#include <math.h>

/* Interrupt header */
#include "app_interrupts.h"

/* Gecko Library */
#ifndef CHECK_TESTING
#include "em_emu.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_usart.h"
#include "em_gpio.h"
#endif;

/* Kinetic */
#include "kinetic.h"

/* System utilities */
#include "usart_sp.h"
#include "clock_sp.h"

/* Peripheral/Sensor headers */
#include "cam_controller.h"
#include "PCA9534A.h"
#include "LSM9DS1.h"
#include "CPT112S.h"

/* Type headers */
#include "kinetic_types.h"
#include "sensor_data_types.h"

#define SYNC_TIMER			TIMER0
#define SYNC_TIMER_PERIOD 	50 * 20

#define FORCE_TIMER			TIMER1
#define FORCE_TIMER_PERIOD 	100

#define _2D_MODE_DEFAULT 	false
#define _3D_MODE_DEFAULT	true
#define _SLEEP_MODE_DEFAULT	false

#define DEFAULT_BEACON_INTENSITY 	100
#define DEFAULT_BEACON_DURATION		1000 /* Milliseconds */

extern app_t 			mode;
extern kinetic_t		kinetics;
extern sensor_data_t 	sensors;
extern LSM9DS1_t        imu0;

/* Callbacks */
void CameraHandler( void );

/* App */
void app_init( 				void );
void app( 					void );
void appModeSet( 	 	 app_t * );

void autoDetectMode( 		void );

void enableSyncTimer( 	sync_t * );
void disableSyncTimer( 		void );
void enableStylusSensors( 	void );
void disableStylusSensors( 	void );
void enableSpatialSensors( 	void );
void disableSpatialSensors( void );
void enterSleepMode( 		void );
void exitSleepMode( 		void );

#endif /* SYSTEM_APP_H_ */
