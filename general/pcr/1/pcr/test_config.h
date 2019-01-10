//
//  test_config.h
//  pcr
//
//  Created by Matthew Fonken on 12/16/18.
//  Copyright © 2018 Matthew Fonken. All rights reserved.
//

#ifndef test_config_h
#define test_config_h

using namespace cv;
using namespace std;

#define MAIN_FPS        60
#define TAU_FPS         60
#define COMBINE_FPS     0

#define TITLE_STRING                    "PCR Alpha v0.01"
#define FRAME_IMAGE_IMAGE_SOURCE_PATH   "frames/noiseS1"// "noise/dynamic_clouds"
#define FRAME_IMAGE_SOURCE_NUM_FRAMES   31

/* GLOBAL SETUP */
//#define HAS_CAMERA
//#define HAS_IMU
//#define HAS_FILE
#define HAS_GENERATOR

#define FRAME_WIDTH                     500
#define FRAME_HEIGHT                    700

/* IMAGE PROCESSING */
#define FRAME_IMAGE_ROOT                "/Users/matthew/Desktop/PersonalResources/TestImages/"

#define sleep(X) usleep(X*1E6)
#define KEY_DELAY 1E3/MAIN_FPS

#define FILENAME                        "/Users/matthew/Desktop/p_data.csv"
#define X_DIM_FILENAME                  "/Users/matthew/Desktop/x_data.csv"
#define Y_DIM_FILENAME                  "/Users/matthew/Desktop/y_data.csv"

#define DASHBOARD_WIDTH                 500
#define DASHBOARD_HEIGHT                FRAME_HEIGHT

#endif /* test_config_h */
