//
//  tau_test.hpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef tau_test_hpp
#define tau_test_hpp

#include "global_types.h"

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#define THRESHOLD   250
#define BRIGHTNESS  100
#define DRAW_HEIGHT 300
#define MAP_INSET   80
#define MAP_SCALE   13
#define FNL_RESIZE  800

using namespace cv;

void drawTau(Mat M, density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r, prediction_pair_t * e);

void print2DArray( pixel_base_t ** i, int w, int h );
void initImg( pixel_base_t *** i, int w, int h );
void drawDensityMap(density_map_pair_t * m);
void drawDensityMapWithPeaks(density_map_pair_t * m, peak_list_pair_t * p);
void drawDensityMapWithProbabilities(density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r);
void drawDensityMapWithPredictions(density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r, prediction_pair_t * e);
bool thresh(uint8_t t, Vec3d p);
void MatToCimage( uint16_t width, uint16_t height, Mat mat, pixel_base_t ** img );
void cimageToMat( uint16_t width, uint16_t height, pixel_base_t ** img, Mat mat );
void cimageToBandW( uint16_t width, uint16_t height, pixel_base_t ** img, Mat mat );
void deriveDensityMap( density_map_t * m );
void gaussianBlur( density_map_t * m );
void smooth1D( density_map_t * m );

#endif /* tau_test_hpp */
