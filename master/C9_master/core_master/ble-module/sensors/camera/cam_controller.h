/*
 * cam_controller.h
 *
 *  Created on: Jan 3, 2017
 *      Author: Matthew Fonken
 */

#ifndef SENSORS_CAMERA_CAM_CONTROLLER_H_
#define SENSORS_CAMERA_CAM_CONTROLLER_H_

#include <stdint.h>
#include <stdbool.h>

/* Application */
#ifdef CHECK_TESTING
#include "../../testing/ble_core_stubs.h"
#else
#include "../../app/app.h"
#include "../../app/app_interrupts.h"

/* Utilities */
#include "clock_sp.h"
#include "usart_sp.h"
#include "rf_controller.h"
#endif

/* Types */
#include "../sensor_data_types.h"
#include "../../system/utilities/data_types.h"

#define CAM_UART		USART0
#define CAM_NULL_CMD	0xff

#define CAMERA_INIT		0xab
#define CENTROID_HEAD	0xee

#define CAMERA_WIDTH	1280
#define	CAMERA_HEIGHT	800
#define CAMERA_ALPHA_W	128
#define	CAMERA_ALPHA_H	80
#define CAMERA_OFFSET_ANGLE_X 0
#define CAMERA_OFFSET_ANGLE_Y 0
#define CAMERA_OFFSET_ANGLE_Z 0

#define MAX_TRACK_COUNT	6

#define MAX_TRACK_AGE	500 // In milliseconds ?

#define MIN_CENTROID_MASS 200

#define MAX_X_DIFF		30
#define MAX_Y_DIFF		30
#define MAX_M_DIFF      50

extern  uint32_t		beacon_vector[2];
extern  centroids_t     centroids;
extern  beacon_t        beacons[MAX_CENTROIDS];
extern  buffer_t        camera_buffer;
void    Print_Beacons(  void );
void 	Camera_Init(	void );
void 	Camera_Read( 	void );
uint8_t Camera_Check(  	void );
uint8_t Camera_Buffer(  uint8_t );
void	Camera_Enable(	void );
void	Camera_Disable(	void );


void	Beacon_Add( 	centroid_t * );
void	Beacon_Check( 	void );
bool 	Beacon_Compare( centroid_t *, centroid_t * );
bool	Beacon_Compose( cartesian2_t [2] );
void 	Beacon_Copy( 	centroid_t *, centroid_t * );
void 	Beacon_Get(     centroid_t [2] );
void	Beacon_Perge( 	void );
void 	Beacon_Sort( 	void );
void 	Beacon_Update( 	uint8_t, centroid_t * );

#endif /* SENSORS_CAMERA_CAM_CONTROLLER_H_ */
