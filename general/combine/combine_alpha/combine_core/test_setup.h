//  test_setup.h
//  tau+
//
//  Created by Matthew Fonken on 10/28/17.
//  Copyright © 2017 Marbl. All rights reserved.

#ifndef test_setup_h
#define test_setup_h

#define TITLE_STRING   "Combine Core v0.1"

#define MAX_RATE  30    //Hz

/* GLOBAL SETUP */

//#define HAS_CAMERA

/* DEBUGGING */
//#define MAIN_DEBUG
//#define STATEM_DEBUG
//#define KMAT_DEBUG
//#define EXT_DEBUG
#define IMG_DEBUG
//#define KIN_DEBUG
#define IMU_DEBUG

//#define REDSCALE
//#define GREYSCALE
#define OPENCV_THRESHOLD

#define TEXT_OFFSET_X 0//-26
#define TEXT_OFFSET_Y 0// 18
#define DETECT_BORDER_OFFSET 10
#define STRENGTH 2.0
#define ZOOM     1.1

/* IMAGE PROCESSING */
#ifdef __APPLE__
#define IMAGE_ROOT              "/Users/matthewfonken/Desktop/CSResources/Test Images/"
#else
#define IMAGE_ROOT              "../"
#endif
#define FRAME_DELAY_MS          10

#define CAM_WIDTH               1280//1920
#define CAM_HEIGHT              720//1080
#define CAM_FRAME_RATE          60
#define FNL_RESIZE_S            1

#ifdef HAS_CAMERA
#define FNL_RESIZE_W            CAM_WIDTH/FNL_RESIZE_S
#define FNL_RESIZE_H            CAM_HEIGHT/FNL_RESIZE_S
#else
#define FNL_RESIZE_W            700
#define FNL_RESIZE_H            700
#endif

#define THRESHOLD_MAX           254
#define THRESHOLD_MIN           250
#define THRESH_STEP             5

#define THRESHOLD_              THRESHOLD_MIN
#define BRIGHTNESS              100
#endif /* test_setup_h */
