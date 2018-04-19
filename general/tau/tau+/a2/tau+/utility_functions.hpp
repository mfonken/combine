//
//  utility_functions.hpp
//  tau+
//
//  Created by Matthew Fonken on 10/6/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef utility_functions_hpp
#define utility_functions_hpp

#include "test_setup.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "global_types.h"

using namespace cv;

static inline bool thresh(uint8_t t, cv::Vec3d p) { return p[0] > t;}

void print2DArray( cimage_t i, int w, int h );
void initImg( cimage_t i, int w, int h );

bool thresh(uint8_t t, Vec3d p);
void MatToCimage( uint16_t width, uint16_t height, Mat mat, cimage_t img, int threshold );
void cimageToBandW( uint16_t width, uint16_t height, cimage_t img, Mat mat );
void deriveDensityMap( density_map_t * m );

#endif /* utility_functions_hpp */
