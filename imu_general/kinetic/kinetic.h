/*! \file spatial.h
    \brief Spatial Orientation Main\r\n

 NOTES:
 - IMU is for rotational data ONLY!
 - Positional data comes from camera triangulation module and beacons
 - Right-handed rotation

  Created by Matthew Fonken on 10/8/16.
 */

#ifndef kinetic_h
#define kinetic_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* Math headers */
#include "kalman.h"
#include "matrix.h"

/* Types */
#include "kinetic_types.h"

/* Utilities */
#include "../tests/test_stubs.h"

/* Sensors headers */
#include "../sensors/LSM9DS1.h"

/***********************************************************************************************//**
 * \defgroup Kinetic Motion Code
 * \brief Functions for Motion Analysis
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup kinetic
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 Global Definitions
***************************************************************************************************/

/** Absolute value */
#define 	absl(x) x > 0 ? x:-x

/** Half PI */
#define     PI      			3.141596
#define 	HALF_PI				PI / 2
#define 	DEG_TO_RAD 			PI / 180

/** Initial normal unit vector to beacon plane */
#define     VISION_ZSTATE_I  0
#define     VISION_ZSTATE_J  0
#define     VISION_ZSTATE_K -1

/** Augmentation skew */
#define     D_AUG               0

/** Physical distance between beacons */
#define     D_FIXED     		1

/***************************************************************************************************
Function Declarations
***************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Initialize Kinetic Sensors
 **************************************************************************************************/
void Kinetic_Init( LSM9DS1_t *, kinetic_t * );

/***********************************************************************************************//**
 *  \brief  Initialize Filters for Kinetic Data
 **************************************************************************************************/
void Filters_Init( LSM9DS1_t * imu, kinetic_t * kinetics );

/***********************************************************************************************//**
 *  \brief  Update rotation filter data
 **************************************************************************************************/
void Kinetic_Update_Rotation( LSM9DS1_t * imu, kinetic_t * kinetics );

/***********************************************************************************************//**
 *  \brief  Update position filter data
 **************************************************************************************************/
void Kinetic_Update_Position( LSM9DS1_t * imu, kinetic_t * kinetics, cartesian2_t beacons[2] );
void Camera_Rotation_Init( void );

/** @} (end addtogroup kinetic) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
}
#endif

#endif /* kinetic_h */
