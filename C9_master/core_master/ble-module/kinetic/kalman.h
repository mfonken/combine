/***********************************************************************************************//**
 * \file   kalman.h
 * \brief  Quick Kalman Filter Header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 *      Sources:
 **************************************************************************************************/

#ifndef kalman_h
#define kalman_h

#ifdef __cplusplus
extern "C" {
#endif

/* Standard headers */
#include <stdio.h>
#include <stdint.h>
    
#ifdef CHECK_TESTING
//#include "../testing/ble_core_stubs.h"
#else
#include "clock_sp.h"
#endif


/* Included types header */
#include "kinetic_types.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup kinetic
 * @{
 **************************************************************************************************/


/***************************************************************************************************
Public Definitions
***************************************************************************************************/

/** Uncertainty of value */
#define VALUE_UNCERTAINTY   0.001
    
/** Uncertainty of bias */
#define BIAS_UNCERTAINTY    0.003
    
/** Uncertainty of sensor */
#define SENSOR_UNCERTAINTY  0.03

/***************************************************************************************************
 Static Function Declarations
 **************************************************************************************************/
 
/***********************************************************************************************//**
*  \brief Initalize Kalman Filter
*  \param[in] k Pointer to kalman filter type
*  \param[in] v Initial value
**************************************************************************************************/
void Kalman_Init( kalman_t *k,
                  double    v );
    
/***********************************************************************************************//**
 *  \brief Update Kalman Filter
 *  \param[in] k Pointer to kalman filter type
 *  \param[in] value_new    New value
 *  \param[in] rate_new     New rate
 *  \param[in] delta_time   Time difference
 **************************************************************************************************/
void Kalman_Update( kalman_t *k,
                    double    value_new,
                    double    rate_new,
					double 	  delta_time );

/** @} (end addtogroup kinetic) */
/** @} (end addtogroup Application) */

#endif /* kalman_h */
