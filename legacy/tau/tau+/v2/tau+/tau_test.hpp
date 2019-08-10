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
#include "utility_functions.hpp"

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#define DRAW_HEIGHT 300
#define MAP_INSET   80
#define MAP_SCALE   13
#define FNL_RESIZE  800

using namespace cv;

void drawTau(Mat M, density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r, prediction_pair_t * e);
void drawKalmanData( prediction_kalman_pair_t * k );

#endif /* tau_test_hpp */
