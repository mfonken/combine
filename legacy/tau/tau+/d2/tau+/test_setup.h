//
//  test_setup.h
//  tau+
//
//  Created by Matthew Fonken on 10/28/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef test_setup_h
#define test_setup_h

#define TITLE_STRING   "Tau+ Demo v1.0"

/* GLOBAL SETUP */
#define HAS_CAMERA
#define CAMERA_ID 1

/* DEBUGGING */
//#define MAIN_DEBUG
//#define TAU_DEBUG
//#define RHO_DEBUG
//#define STATEM_DEBUG
//#define IMU_DEBUG

#ifdef HAS_CAMERA
#define FISHEYE
#endif

#define STRENGTH 2.0
#define ZOOM     0.7

/* PROBABILITY BIASES */
#define LOCATION_BIAS           0.5
#define DENSITY_BIAS            1 - LOCATION_BIAS

/* IMAGE PROCESSING */
#ifdef __APPLE__
#define IMAGE_ROOT              "/Users/matthewfonken/Desktop/CSResources/Test Images/"
#else
#define IMAGE_ROOT              "../"
#endif

#define DATA_FILENAME          "/Users/matthewfonken/Desktop/out.txt"
#define RATES_FILENAME         "/Users/matthewfonken/Desktop/rates.txt"

#define DEFAULT_COVERAGE        60

#define CAM_WIDTH               1280
#define CAM_HEIGHT              720
#define CAM_FRAME_RATE          60
#define FNL_RESIZE_S            2

#ifdef HAS_CAMERA
#define FNL_RESIZE_W            CAM_WIDTH/FNL_RESIZE_S
#define FNL_RESIZE_H            CAM_HEIGHT/FNL_RESIZE_S
#else
#define FNL_RESIZE_W            700
#define FNL_RESIZE_H            700
#endif

#define FNL_RESIZE_W_HALF       FNL_RESIZE_W/2
#define FNL_RESIZE_H_HALF       FNL_RESIZE_H/2

#define THRESHOLD_MAX           240
#define THRESHOLD_MIN           150
#define THRESH_STEP             1

#define THRESHOLD_              205
#define BRIGHTNESS              155

/* TAU SETUP */
#define DRAW_HEIGHT             300
#define MAP_INSET               80
#define MAP_SCALE               1
#define FNL_RESIZE              500

/* RHO SETUP */
#define DEFAULT_GAUSS_LEN       15
#define DEFAULT_SIGMA           3.0 //GAUSSIAN SIGMA
#define MAX_PEAKS_RHO           10
#define PEAK_LIST_SCAN_STEP     5

#endif /* test_setup_h */
