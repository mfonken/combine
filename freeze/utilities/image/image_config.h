//
//  image_config.h
//  pcr
//
//  Created by Matthew Fonken on 12/15/18.
//  Copyright © 2018 Matthew Fonken. All rights reserved.
//

#ifndef image_config_h
#define image_config_h

#include "test_config.h"

#define IMAGE_ROOT              FRAME_IMAGE_ROOT

//#define TILT_LINES
//#define GREYSCALE

#define THRESH_IMAGE

#define PATH_NUM_TICKS  200
#define PATH_OFFSET     200
#define TARGET_RADIUS   20
#define DEFAULT_PATH    CIRCLE_DUAL
#define TARGET_COLOR    Scalar(215,215,215)
#define PATH_OFFSET_A   0.2
#define PATH_OFFSET_B   1.1
#define PATH_OFFSET_C   1.5

#define DEFAULT_NOISE   NONE//STATIC_SMALL
#define NOISE_RATE      10  //Hz
#define NOISE_COLOR     Scalar(200,200,200)
#define NOISE_ORIGIN_X  50
#define NOISE_ORIGIN_Y  200
#define NOISE_WIDTH     150
#define NOISE_HEIGHT    100

#define IU_WIDTH        FRAME_WIDTH
#define IU_HEIGHT       FRAME_HEIGHT
#define IU_FRAME_RATE   60

#define IU_THRESHOLD    100
#define IU_BRIGHTNESS   255

// ?
#define     D_FIXED                 5.0e-2


#endif /* image_config_h */
