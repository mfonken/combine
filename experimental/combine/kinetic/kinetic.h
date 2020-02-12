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

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "test_config.h"
#include "genlog.h"

/* Math headers */
#include "qmath.h"
#include "matvec.h"

/* Filters */
#ifdef __KALMAN__
#include "kalman.h"

//#define KINETIC_USE_QUATERNION_INPUT
#define KINETIC_USE_POINT_TRANSLATION_ROTATION_CORRECTION

#define CYCLE3(X) for(uint8_t X=0;X<3;X++)

/* Kalman Defaults */
#define MOTION_MAX_KALMAN_LIFESPAN 10.0
#define MOTION_VALUE_UNCERTAINTY   0.2
#define MOTION_BIAS_UNCERTAINTY    0.001
#define MOTION_SENSOR_UNCERTAINTY  0.01
#endif

#define ROTATION_MAX               (2. * M_PI)
#define ROTATION_MIN               (-ROTATION_MAX)
#define POSITION_MIN               -100
#define POSITION_MAX               1000

#define DEFAULT_MOTION_UNCERTAINTY \
(kalman_uncertainty_c){ MOTION_VALUE_UNCERTAINTY, MOTION_BIAS_UNCERTAINTY, MOTION_SENSOR_UNCERTAINTY }

/** Absolute value */
#define     absl(x) x > 0 ? x:-x

/** Initial normal unit vector to beacon plane */
//#define     VISION_ZSTATE_I  0
//#define     VISION_ZSTATE_J  0
//#define     VISION_ZSTATE_K -1

/* Camera Precalculated values */
#define     CAMERA_OFFSET_ANGLE_X   0       // radians
#define     CAMERA_OFFSET_ANGLE_Y   0       // radians
#define     CAMERA_OFFSET_ANGLE_Z   0       // radians

#define     REFERENCE_OFFSET_ANGLE_X   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Y   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Z   0//-110 * DEG_TO_RAD // radians

//#define     CAMERA_ALPHA_W          120     // degrees
//#define     CAMERA_ALPHA_H          75      // degrees

#define     KINETIC_CAMERA_WIDTH    FRAME_WIDTH//1280    // pixels
#define     KINETIC_CAMERA_HEIGHT   FRAME_HEIGHT//800     // pixels
#define     CAMERA_HALF_WIDTH       FNL_RESIZE_W/2
#define     CAMERA_HALF_HEIGHT      FNL_RESIZE_H/2

#define     UNITS                  "cm"
#define     PIXEL_TO_UNIT           3e-6
#define     UNIT_TO_PIXEL         ( 1.0 / PIXEL_TO_UNIT )

//#define     CAMERA_HALF_WIDTH_UNIT  CAMERA_HALF_WIDTH*PIXEL_TO_UNIT
//#define     CAMERA_HALF_HEIGHT_UNIT CAMERA_HALF_HEIGHT*PIXEL_TO_UNIT

#define     FOCAL_REFRACTION        2.4e-6//1.3e-6//  // units / units
#define     FOCAL_LENGTH          ( 1.39e-3 * FOCAL_REFRACTION ) * UNIT_TO_PIXEL//( 3.5e-3 * FOCAL_REFRACTION ) * UNIT_TO_PIXEL // dimension units

#define     D_FIXED                 5.0e-2

#define KIN_DEBUG DEBUG_1

#ifdef KIN_DEBUG
#define LOG_KIN(L,...)          LOG(L,"<Kinetic> " __VA_ARGS__)
#define LOG_KIN_BARE(L,...)     LOG_BARE(L,"" __VA_ARGS__)
#else
#define LOG_KIN(...)
#define LOG_KIN_BARE(L,...)
#endif


/** Kinetic Type */
typedef struct _kinetic_values_t
{
    floating_t      position[3];             /**< Raw position */
    double          rotation[3];             /**< Raw rotation */
} kinetic_values_t;

#ifdef __KALMAN__
typedef struct _kinetic_filters_t
{
    kalman_filter_t position[3];             /**< Raw position */
    kalman_filter_t rotation[3];             /**< Raw rotation */
    kalman_filter_t point_rotation[3];             /**< Raw rotation */
} kinetic_filters_t;
#endif

typedef struct _kinetic_t
{
    kinetic_values_t values;
#ifdef __KALMAN__
    kinetic_filters_t filters;
#endif
    kpoint_t
    A,
    B,
    A_,
    B_;
    vec3_t
    AB_;
    floating_t  omega,
    sigmaA,
    sigmaR,
    nu,
    upsilon,
    chi,
    mu,
    gamma;
    int     W,
    H;
    floating_t  f_l,
    d_l,
    d__l,
    r_l;
    ang3_t  raw,
            e;
    vec3_t  r,
    n;
    quaternion_t
    qd,
    qd_,
//    qs,
    qs_,
    qc,
    qc_,
    qr,
//    qg,
    qa;
} kinetic_t;

typedef struct _serial_kinetic_t
{
    ang3_t  orientation;
    vec3_t  position;
    struct timeval timestamp;
} serial_kinetic_t;

typedef struct
{
    void  (*DefaultInit)(kinetic_t * );
    void  (*Init)( kinetic_t *, int, int, floating_t, floating_t);
    void  (*UpdateRotation)( kinetic_t *,
#ifdef KINETIC_USE_QUATERNION_INPUT
                            quaternion_t *,
#else
                            ang3_t *,
#endif
                            ang3_t *
#ifdef KINETIC_USE_POINT_TRANSLATION_ROTATION_CORRECTION
                            , kpoint_t *, kpoint_t *
#endif
                            );
    void  (*UpdatePosition)( kinetic_t *, vec3_t *, kpoint_t *, kpoint_t * );
    
    ang3_t (*PointTranslationParameters)( kinetic_t *, kpoint_t *, kpoint_t * );
    void (*MinorAngles)( kinetic_t *, kpoint_t *, kpoint_t * );
    void (*Quaternions)( kinetic_t * );
    void (*MajorAngles)( kinetic_t * );
    int  (*Chi)( kinetic_t * );
    int  (*Mu)( kinetic_t * );
    void (*Gamma)( kinetic_t * );
    int  (*R_l)( kinetic_t * );
    void (*R)( kinetic_t * );
    void (*Nongrav)( kinetic_t *, vec3_t * );
    floating_t (*DeltaR)( floating_t, vec3_t *, vec3_t * );
} kinetic_functions;

extern kinetic_functions KineticFunctions;

#ifdef __KALMAN__
void Filters_Init( kinetic_t *);
#endif
void Camera_Rotation_Init( kinetic_t *);
void Reference_Rotation_Init( kinetic_t *);

#endif /* kinetic_h */

