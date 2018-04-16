//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#include "test_setup.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>
#include <sys/time.h>

#include "global_types.h"
#include "tau_test.hpp"
#include "statem.h"
#include "tau.h"
#include "rho.h"

#include "image_utility.hpp"

using namespace cv;
using namespace std;

int num_orders;
int orders[] = { 10, 100, 500 };//1000, 10000, 100000 };

int main( int argc, const char * argv[] )
{
    num_orders = sizeof(orders)/sizeof(orders[0]);
#ifdef TIME_FULL_LOOP
    struct timeval start,stop;
#endif
    
    image_test util(argc, argv);
    
    int width = util.getWidth();
    int height = util.getHeight();

    tau_t tau;
    initTauA(&tau, width, height );
    
//    int size = sizeof(prediction_pair_t);
//    printf("Size of tau is %dbytes\n", size);

    pixel_base_t pixel_array[width*height];
    cimage_t pixels = pixel_array;
    
    int threshold = THRESHOLD;
    double t[3];

#ifdef ITERATIONS
    for(int o = 0; o < num_orders; o++)
    {
    int iterations = orders[o];
    double times[iterations];
    printf("Running for %d iterations\n", iterations);
    for(int l=0;l<iterations;l++) {
#else
    printf("Running with User Control\n");
    for(int l=0;l<1;) {
#endif
        
#ifdef TIME_FULL_LOOP
        gettimeofday( &start, NULL);
#endif
        
        /* Re-init frame and out images every loop */
        Mat frame, out(height, width, CV_8UC3, Scalar(0,0,0));
        frame = util.getNextFrame();
        
        /* Convert image to filtered pixel array and back to black and white image */
        MatToCimage( width, height, frame, pixels, threshold );
        cimageToBandW( width, height, pixels, out );

        /* Run Tau */
        performTauA(&tau, t, pixels);
        
#ifdef ITERATIONS
        times[l] = t[0]+t[1]+t[2];
#endif
        
        /* Update threshold */
        int n = stateNumber(tau.sys.state);
        if( n > NUM_OF_SELECTIONS ) threshold += THRESH_STEP;
        if( n < NUM_OF_SELECTIONS ) threshold -= THRESH_STEP;
        if(threshold>THRESHOLD_MAX) threshold = THRESHOLD_MAX;
        if(threshold<THRESHOLD_MIN) threshold = THRESHOLD_MIN;
#ifdef MAIN_DEBUG
        printf("Threshold for state %s is %d. - (%d)\n", stateString(sys.state), threshold, n );
#endif
        
#ifdef STATEM_DEBUG
        printBayesianMap(&tau.sys.probabilities, tau.sys.state);
#endif
#ifdef SHOW_IMAGES
        /* Draw time division array */
        double a[] = { 0.0, t[0], t[0] + t[1], t[0] + t[1] + t[2]};
        drawTiming(a, sizeof(a)/sizeof(a[0]));
        
        /* Update FPS display on main output image */
        string text = "FPS:" + to_string(1/a[3]);
        cv::putText(out, text, Point(3, 18), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(255), 1, 8);
        
        /* Draw Tau output image */
        drawTau(out, &tau.rho.density_map_pair, &tau.rho.peak_list_pair, &tau.predictions);
//        drawDensityMaps(&tau.rho.density_map_pair);
#endif
        
#ifdef TIME_FULL_LOOP
        gettimeofday( &stop, NULL);
        printf("Total loop time is %f (s)\n", timeDiff(start, stop));
#endif
        
        /* Wait for timer or key press */
#ifdef SHOW_IMAGES
        util.loop(waitKey(util.isLive()?FRAME_DELAY_MS:60000));
#else
//        printf("#%d\n", l);
        util.loop(' ');
#endif
    }
#ifdef ITERATIONS
    long double average = 0;
        for(int l = 0; l < iterations; l++) average += times[l];
        average /= iterations;
        printf("L-%d A-%Lf\n", iterations, average);
    }
#endif
    return 0;
}
