//
//  test_setup.h
//  tau+
//
//  Created by Matthew Fonken on 10/28/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef test_setup_h
#define test_setup_h

#define TITLE_STRING   "Tau+ Alpha v0.1"

/* GLOBAL SETUP */
//#define SHOW_IMAGES
//#define HAS_CAMERA
//#define TIME_FULL_FRAME

/* DEBUGGING */
//#define MAIN_DEBUG
//#define RHO_DEBUG
//#define STATEM_DEBUG
//#define KMAT_DEBUG
//#define EXT_DEBUG
//#define IMUT_DEBUG

#ifndef SHOW_IMAGES
#define ITERATIONS
#endif

#define MAX_PEAKS               3

#define INIT_ITERATIONS         100     // Cycles

#define CHAOS                   5
#define NUM_OF_SELECTIONS       1

#define TEXT_OFFSET_X 0//-26
#define TEXT_OFFSET_Y 0// 18
#define DETECT_BORDER_OFFSET 10
#define STRENGTH 2.0
#define ZOOM     1.1

/* PROBABILITY BIASES */
#define LOCATION_BIAS           0.5
#define DENSITY_BIAS            1 - LOCATION_BIAS

/* IMAGE PROCESSING */
#ifdef __APPLE__
#define IMAGE_ROOT              "/Users/matthewfonken/Desktop/CSResources/Test Images/"
#else
#define IMAGE_ROOT              "../"
#endif

#define FILENAME                "/Users/matthewfonken/Desktop/out.txt"

#define FRAME_DELAY_MS          100

#define CAM_WIDTH               1280//1920
#define CAM_HEIGHT              720//1080
#define CAM_FRAME_RATE          60
#define FNL_RESIZE_S            2
#define FNL_RESIZE_W            700//CAM_WIDTH/FNL_RESIZE_S
#define FNL_RESIZE_H            700//CAM_HEIGHT/FNL_RESIZE_S

#define THRESHOLD_MAX           254
#define THRESHOLD_MIN           150
#define THRESH_STEP             1

#define THRESHOLD               222
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
