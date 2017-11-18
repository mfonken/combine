//
//  test_setup.h
//  tau+
//
//  Created by Matthew Fonken on 10/28/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

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
//#define IMUT_DEBUG
//#define KIN_DEBUG

#ifndef SHOW_IMAGES
#define ITERATIONS
#endif

/* KALMAN / KMAT SETUP */
#define MAX_KALMANS             3
#define MAX_PEAKS               5
//#define KALMAN_IS_TIMELESS            // NOTE: Currently breaks kalman gain and thus state machine
#define MAX_DENSITY_DIFF        20      // Pixels
#define KALMAN_SORT_SCALE_STEP  1       // Pixels
#define KALMAN_PAIR_MAX         3       // Pair/sorting steps
#define MIN_PROB                0.1     // Gain units
#define SELECTION_THRESHOLD     0.01    // Gain units
#define MIN_PEAK                20      // Pixels
#define SIMILARITY_TOLERANCE    10      // Pixels
#define PUNISH_FACTOR           0.8     // Gain / Gain

#define CHAOS                   5
#define NUM_OF_SELECTIONS       1

/* PROBABILITY BIASES */
#define LOCATION_BIAS           0.5
#define DENSITY_BIAS            1 - LOCATION_BIAS

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
#define THRESHOLD_MIN           100
#define THRESH_STEP             5

#define THRESHOLD               THRESHOLD_MIN
#define BRIGHTNESS              100

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
