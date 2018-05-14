/***********************************************************************************************//**
 * \file   matrix.h
 * \brief  Matrix Math Header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef matrix_h
#define matrix_h

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

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
 Static Function Declarations
 **************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Subract two 3D vectors
 *  \param[in] x Subracted from and returned
 *  \param[in] y Values to subract
 **************************************************************************************************/
void subtractvec3_t( vec3_t * x, vec3_t * y );

/***********************************************************************************************//**
 *  \brief  Return length of 3D vector
 *  \param[out] Length of vector
 *  \param[in] x Vector measured
 **************************************************************************************************/
double lengthOfvec3_t( vec3_t * vec );
double lengthOfvec2_t( vec2_t * vec );

/***********************************************************************************************//**
 *  \brief  Normalize a 3D Vector
 *  \param[in] vec Vector to normalize, returned as self
 **************************************************************************************************/
void normalizevec3_t( vec3_t * vec );

/***********************************************************************************************//**
 *  \brief  Distance between two 2D coordinates
 *  \param[out] Distance between
 *  \param[in] a First coordinate
 *  \param[in] b Second coordinate
 **************************************************************************************************/
double get2dDistance( cartesian2_t *a, cartesian2_t *b );

/** @} (end addtogroup kinetic) */
/** @} (end addtogroup Application) */

void Multiply_Vec_3x1( mat3x3_t * a, vec3_t * b, vec3_t * c );
void Euler_To_Quaternion( ang3_t * ang, quaternion_t * quat );
void Quaternion_To_Matrix(quaternion_t * quat, mat3x3_t * m);
void Quaternion_Combine(quaternion_t * a, quaternion_t * b, quaternion_t * c, quaternion_t * d );
void cross3( vec3_t * u, vec3_t * v, vec3_t * r );
void mul3( double m, vec3_t * v, vec3_t * r );
void add33( vec3_t * u, vec3_t * v, vec3_t * w, vec3_t * r);
void Rotate_Vector_By_Quaternion(vec3_t * v, quaternion_t * q, vec3_t * r);

#endif /* matrix_h */
