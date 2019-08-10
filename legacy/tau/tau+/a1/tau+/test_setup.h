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

/* DEBUGGING */
//#define MAIN_DEBUG
//#define STATEM_DEBUG
//#define KMAT_DEBUG
//#define EXT_DEBUG
//#define IMUT_DEBUG
#define SHOW_IMAGES

#ifndef SHOW_IMAGES
#define ITERATIONS 10000
#endif

/* KALMAN / KMAT SETUP */
#define MAX_KALMANS 3
#define MAX_PEAKS   5
//#define KALMAN_IS_TIMELESS            // NOTE: Currently breaks kalman gain and thus state machine
#define MAX_DENSITY_DIFF        20      // Pixels
#define KALMAN_SORT_SCALE_STEP  1       // Pixels
#define KALMAN_PAIR_MAX         3       // Pair/sorting steps
#define MIN_PROB                0.1     // Gain units
#define SELECTION_THRESHOLD     0.01    // Gain units
#define MIN_PEAK                50      // Pixels
#define SIMILARITY_TOLERANCE    10      // Pixels
#define PUNISH_FACTOR           0.8     // Gain / Gain

#define INIT_ITERATIONS         100     // Cycles

#define CHAOS                   5
#define NUM_OF_SELECTIONS       1

/* PROBABILITY BIASES */
#define LOCATION_BIAS   0.5
#define DENSITY_BIAS    1 - LOCATION_BIAS

/* IMAGE PROCESSING */
#define IMAGE_ROOT "/Users/matthewfonken/Desktop/test images/"
#define FRAME_DELAY_MS 100
#define CAM_WIDTH   640
#define CAM_HEIGHT  320

#define THRESHOLD_MAX   254
#define THRESHOLD_MIN   150
#define THRESH_STEP     5

#define THRESHOLD   THRESHOLD_MIN
#define BRIGHTNESS  100

/* TAU SETUP */
#define DRAW_HEIGHT 300
#define MAP_INSET   80
#define MAP_SCALE   13
#define FNL_RESIZE  800

/* RHO SETUP */
#define DEFAULT_SIGMA 3.0 //GAUSSIAN SIGMA

#endif /* test_setup_h */
