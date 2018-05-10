//  test_setup.h
//  tau+
//
//  Created by Matthew Fonken on 10/28/17.
//  Copyright © 2017 Marbl. All rights reserved.

#ifndef test_setup_h
#define test_setup_h

#define TITLE_STRING   "Combine Core v0.1"

/* GLOBAL SETUP */
#define SHOW_IMAGES
#define HAS_CAMERA
//#define TIME_FULL_LOOP


/* DEBUGGING */
//#define MAIN_DEBUG
//#define STATEM_DEBUG
//#define KMAT_DEBUG
//#define EXT_DEBUG
#define IMG_DEBUG
//#define KIN_DEBUG
//#define IMU_DEBUG

#ifndef SHOW_IMAGES
#define ITERATIONS
#endif

#define REDSCALE
//#define GREYSCALE
#define OPENCV_THRESHOLD
#define FNL_RESIZE 300

/* IMAGE PROCESSING */
#ifdef __APPLE__
#define IMAGE_ROOT              "/Users/matthewfonken/Desktop/test images/"
#else
#define IMAGE_ROOT              "../"
#endif
#define FRAME_DELAY_MS          100
#define CAM_WIDTH               640
#define CAM_HEIGHT              320

#define THRESHOLD_MAX           254
#define THRESHOLD_MIN           150
#define THRESH_STEP             5

#define THRESHOLD_              THRESHOLD_MIN
#define BRIGHTNESS              100
#endif /* test_setup_h */
