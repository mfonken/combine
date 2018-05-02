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
#include <sys/time.h>

/* Math headers */
#include "qmath.h"
#include "test_setup.h"

/* Filters */
#include "filters/kalman.h"

/* Kalman Defaults */
#define MOTION_MAX_KALMAN_LIFE     10.0
#define MOTION_VALUE_UNCERTAINTY   0.01
#define MOTION_BIAS_UNCERTAINTY    0.003
#define MOTION_SENSOR_UNCERTAINTY  0.02
    
/** Initial normal unit vector to beacon plane */
#define     VISION_ZSTATE_I  0
#define     VISION_ZSTATE_J  0
#define     VISION_ZSTATE_K -1
    
/* Camera Precalculated values */
#define     CAMERA_OFFSET_ANGLE_X   0       // radians
#define     CAMERA_OFFSET_ANGLE_Y   0       // radians
#define     CAMERA_OFFSET_ANGLE_Z   0       // radians
    
#define     REFERENCE_OFFSET_ANGLE_X   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Y   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Z   -110 * DEG_TO_RAD //M_PI_2 // radians
    
//#define     CAMERA_ALPHA_W          120     // degrees
//#define     CAMERA_ALPHA_H          75      // degrees
    
//#define     CAMERA_WIDTH            960//1280    // pixels
//#define     CAMERA_HEIGHT           540//800     // pixels
#define     CAMERA_HALF_WIDTH       FNL_RESIZE_W/2
#define     CAMERA_HALF_HEIGHT      FNL_RESIZE_H/2

#ifdef HAS_CAMERA
#define     UNITS                  "cm"
#define     PIXEL_TO_UNIT           3e-6            // meters
//#define     FOCAL_REFRACTION        1.3e-6        // units / units
//#define     FOCAL_LENGTH          ( 3.5e-3 * FOCAL_REFRACTION ) * UNIT_TO_PIXEL // dimension units
#define     FOCAL_REFRACTION        2.4e-6          // units / units
#define     FOCAL_LENGTH          ( 1.39e-3 * FOCAL_REFRACTION ) * UNIT_TO_PIXEL // dimension units
#else
#define     PIXEL_TO_UNIT           1
#define     FOCAL_LENGTH            260
#endif
#define     UNIT_TO_PIXEL         ( 1.0 / PIXEL_TO_UNIT )
    
#define     CAMERA_HALF_WIDTH_UNIT  CAMERA_HALF_WIDTH*PIXEL_TO_UNIT
#define     CAMERA_HALF_HEIGHT_UNIT CAMERA_HALF_HEIGHT*PIXEL_TO_UNIT
    

    
#define     D_FIXED                 5.7e-2          // meters
    
    typedef struct timeval timeval;
    static double seconds_since( double time )
    {
        timeval now;
        gettimeofday( &now, NULL );
        return (now.tv_sec + now.tv_usec/1000000.0) - time;
    }
    
    /** Kinetic Type */
    typedef struct _kinetic_values_t
    {
        double     position[3];             /**< Raw position */
        double     rotation[3];             /**< Raw rotation */
    } kinetic_values_t;
    
    typedef struct _kinetic_filters_t
    {
        kalman_t position[3];             /**< Raw position */
        kalman_t rotation[3];             /**< Raw rotation */
    } kinetic_filters_t;

    typedef struct _kinetic_t
    {
        kinetic_values_t values;
        kinetic_filters_t filters;
        kpoint_t
                A,
                B;
        double  omega,
                sigmaA,
                sigmaR,
                nu,
                upsilon,
                chi,
                mu,
                gam;
        int     W,
                H;
        double  f_l,
                d_l,
                d__l,
                r_l;
        ang3_t  e;
        vec3_t  r,
                n;
        
        /* Beacon(T-True) > Reference > Device > Camera to Beacon(V-visualized) */
        quaternion_t
                qr,     /* Beacon(T) > Reference */
                qd,     /* Reference > Device    */
                qr_,    /* Beacon(T) > Device    */
                qc,     /* Device    > Camera    */
                qd_,    /* Beacon(T) > Camera    */
                qc_,    /* Camera    > Beacon(V) */
                qa;     /* Beacon(T) > Beacon(V) */
        ang3_t  offset;
    } kinetic_t;
    
    typedef struct _serial_kinetic_t
    {
        ang3_t  orientation;
        vec3_t  position;
        timeval timestamp;
    } serial_kinetic_t;
    
    typedef struct kinetic kinetic;
    struct kinetic
    {
        int  (*init)( kinetic_t *, int, int, double, double);
        void (*updateRotation)(  kinetic_t *, ang3_t * , ang3_t *);
        void (*updatePosition)(  kinetic_t *, vec3_t *, kpoint_t * , kpoint_t * );
        void (*updateReference)( kinetic_t *, ang3_t * );
        
        void (*minorAngles)( kinetic_t *, kpoint_t * , kpoint_t * );
        void (*quaternions)( kinetic_t * );
        void (*majorAngles)( kinetic_t * );
        int  (*chi)( kinetic_t * );
        int  (*mu)( kinetic_t * );
        void (*gam)( kinetic_t * );
        int  (*r_l)( kinetic_t * );
        void (*r)( kinetic_t * );
        void (*nongrav)( kinetic_t *, vec3_t * );
    };
    
    extern const kinetic Kinetic;
    
    
void Filters_Init( kinetic_t *);
void Camera_Rotation_Init( kinetic_t *);
void Reference_Rotation_Init( kinetic_t *);

    
#ifdef __cplusplus
};
#endif
#endif /* kinetic_h */
