/***********************************************************************************************//**
 * \file   kinetic_types.h
 * \brief  Types for kinetic math and filtering
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef KINETIC_KINETIC_TYPES_H_
#define KINETIC_KINETIC_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup kinetic
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 Public Types
 ***************************************************************************************************/

typedef double mfloat;

/** Kalman structure */
typedef struct _kalman_t
{
    mfloat      K[2];               /**< Gain matrix */
    mfloat      P_k[2][2];          /**< Error covariance matrix */
    mfloat      rate;               /**< Rate */
    mfloat      bias;               /**< Bias */
    mfloat      value;              /**< Value */
    uint32_t    timestamp;          /**< Timestamp */
} kalman_t;

/** Kinetic Type */
typedef struct _kinetic_t
{
    mfloat     position[3];             /**< Raw position */
    mfloat     rotation[3];             /**< Raw rotation */
    mfloat     truePosition[3];         /**< Raw true location */

    kalman_t   positionFilter[3];       /**< Filtered position */
    kalman_t   rotationFilter[3];       /**< Filtered rotation */
    kalman_t   truePositionFilter[3];   /**< Filtered true location */
} kinetic_t;

/** 2D Cartesian Cooridinate */
typedef struct _cartesian2_t
{
    mfloat x;                   /**< X value */
    mfloat y;                   /**< Y value */
} cartesian2_t;

/** Tait-Bryan Angles */
typedef struct _ang3_t
{
    mfloat    x;                /**< Phi value */
    mfloat    y;                /**< Theta value */
    mfloat    z;                /**< Psi value */
} ang3_t;

/*! Simple 3d Vector */
typedef struct _vec3_t
{
    mfloat    i;             /**< \$f\hat{i}\$f value */
    mfloat    j;             /**< \$f\hat{j}\$f value */
    mfloat    k;             /**< \$f\hat{k}\$f value */
} vec3_t;

/*! Simple 3d Vector */
typedef struct _mat3x3_t
{
    mfloat    ii;               /**< \$f\hat{ii}\$f value */
    mfloat    ji;               /**< \$f\hat{ji}\$f value */
    mfloat    ki;               /**< \$f\hat{ki}\$f value */
    mfloat    ij;               /**< \$f\hat{ij}\$f value */
    mfloat    jj;               /**< \$f\hat{jj}\$f value */
    mfloat    kj;               /**< \$f\hat{kj}\$f value */
    mfloat    ik;               /**< \$f\hat{ik}\$f value */
    mfloat    jk;               /**< \$f\hat{jk}\$f value */
    mfloat    kk;               /**< \$f\hat{kk}\$f value */
} mat3x3_t;

/*! Simple 2d Vector */
typedef struct _vec2_t
{
    mfloat    i;             /**< \$f\hat{i}\$f value */
    mfloat    j;             /**< \$f\hat{j}\$f value */
} vec2_t;

/*! Quaternion hypercomplex type */
typedef struct
{
    mfloat x;
    mfloat y;
    mfloat z;
    mfloat w;
} quaternion_t;

/** @} (end addtogroup kinetic) */
/** @} (end addtogroup Application) */

#ifdef __cplusplus
}
#endif

#endif /* KINETIC_KINETIC_TYPES_H_ */
