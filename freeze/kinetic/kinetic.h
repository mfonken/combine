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

//#include "test_setup.h"

/* Math headers */
#include "qmath.h"

/* Filters */
#include "kalman.h"

#include "unilog.h"

//#define DEBUG_KINETIC
#define KINETIC_LOG_PRIO DEBUG_2

#ifdef DEBUG_KINETIC
#define LOG_KINETIC(L, ...) LOG(L, "<Kinetic> " __VA_ARGS__)
#else
#define LOG_KINETIC(L, ...)
#endif


/* Camera Precalculated values */
#define     CAMERA_OFFSET_ANGLE_X   0       // radians
#define     CAMERA_OFFSET_ANGLE_Y   0       // radians
#define     CAMERA_OFFSET_ANGLE_Z   0       // radians

#define     REFERENCE_OFFSET_ANGLE_X   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Y   0    // radians
#define     REFERENCE_OFFSET_ANGLE_Z   -110 * DEG_TO_RAD //M_PI_2 // radians

//#define     CAMERA_ALPHA_W          120     // degrees
//#define     CAMERA_ALPHA_H          75      // degrees

#define     KINETIC_CAMERA_WIDTH    1//CAMERA_WIDTH//1280    // pixels
#define     KINETIC_CAMERA_HEIGHT   1//CAMERA_HEIGHT//800     // pixels
#define     CAMERA_HALF_WIDTH       FNL_RESIZE_W/2
#define     CAMERA_HALF_HEIGHT      FNL_RESIZE_H/2

#define     UNITS                  "cm"
#define     PIXEL_TO_UNIT           1.0//3e-6
#define     UNIT_TO_PIXEL         ( 1.0 / PIXEL_TO_UNIT )

#define     FOCAL_LENGTH            463.7 // pixels

#define     D_FIXED                 5.0e-2

/** Kinetic Type */
typedef struct _kinetic_values_t
{
    double     position[3];             /**< Raw position */
    double     rotation[3];             /**< Raw rotation */
} kinetic_values_t;

typedef struct _kinetic_t
{
    kinetic_values_t values;
    kpoint_t A;
    kpoint_t B;
    double omega;
    double sigmaA;
    double sigmaR;
    double nu;
    double upsilon;
    double chi;
    double mu;
    double gamma;
    int W;
    int H;
    double f_l;
    double d_l;
    double d__l;
    double r_l;
    ang3_t raw;
    ang3_t e;
    vec3_t r;
    vec3_t n;
    quaternion_t qd;
    quaternion_t qd_;
    quaternion_t qs;
    quaternion_t qs_;
    quaternion_t qc;
    quaternion_t qc_;
    quaternion_t qr;
    quaternion_t qg;
    quaternion_t qa;
} kinetic_t;

typedef struct _serial_kinetic_t
{
    ang3_t  orientation;
    vec3_t  position;
    struct timeval timestamp;
} serial_kinetic_t;

typedef struct
{
    int width;
    int height;
    double focal_length;
    double beacon_distance;
} kinetic_config_t;

typedef struct
{
    void (*DefaultInit)(kinetic_t * );
    void (*Init)( kinetic_t *, kinetic_config_t * );
    void (*UpdatePosition)( kinetic_t *, quaternion_t *, kpoint_t * , kpoint_t * );
    
    void (*MinorAngles)( kinetic_t *, kpoint_t * , kpoint_t * );
    void (*Quaternions)( kinetic_t *, quaternion_t * );
    void (*MajorAngles)( kinetic_t * );
    int  (*Chi)( kinetic_t * );
    int  (*Mu)( kinetic_t * );
    void (*Gamma)( kinetic_t * );
    int  (*R_l)( kinetic_t * );
    void (*R)( kinetic_t * );
    void (*Nongrav)( kinetic_t *, vec3_t * );
    void (*Print)( kinetic_t * );
} kinetic_functions;

extern kinetic_functions KineticFunctions;

void Camera_Rotation_Init( kinetic_t *);
void Reference_Rotation_Init( kinetic_t *);

#endif /* kinetic_h */

